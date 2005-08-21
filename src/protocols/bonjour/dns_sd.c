/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <string.h>

#include "dns_sd.h"
#include "bonjour.h"
#include "buddy.h"
#include "debug.h"

// Private data

typedef struct _dns_sd_packet{
	gchar* name;
	gchar* txtvers;
	gchar* version;
	gchar* first;
	gchar* last;
	gint port_p2pj;
	gchar* phsh;
	gchar* status;
	gchar* message;
	gchar* email;
	gchar* vc;
	gchar* jid;
	gchar* AIM;
}dns_sd_packet;

// End private data

// Private functions

static sw_result HOWL_API _publish_reply(sw_discovery discovery, 
	sw_discovery_oid oid, sw_discovery_publish_status status, sw_opaque extra)
{
gaim_debug_warning("bonjour", "_publish_reply --> Start\n");
	// Check the answer from the mDNS daemon
	switch(status){
		case SW_DISCOVERY_PUBLISH_STARTED :
			gaim_debug_info("bonjour", "_publish_reply --> Service started\n");
			break;
		case SW_DISCOVERY_PUBLISH_STOPPED :
			gaim_debug_info("bonjour", "_publish_reply --> Service stopped\n");
			break;
		case SW_DISCOVERY_PUBLISH_NAME_COLLISION :
			gaim_debug_info("bonjour", "_publish_reply --> Name collision\n");
			break;
		case SW_DISCOVERY_PUBLISH_INVALID :
			gaim_debug_info("bonjour", "_publish_reply --> Service invalid\n");
			break;
	}
	
	return SW_OKAY;
}

static sw_result HOWL_API _resolve_reply(sw_discovery discovery, 
	sw_discovery_oid oid, sw_uint32 interface_index, sw_const_string name, 
	sw_const_string type, sw_const_string domain, sw_ipv4_address address, 
	sw_port port, sw_octets text_record, sw_ulong text_record_len, 
	sw_opaque extra)
{
	BonjourBuddy* buddy;
	GaimAccount* account = (GaimAccount*)extra;
	gchar* txtvers = NULL;
	gchar* version = NULL;
	gchar* first = NULL;
	gint port_p2pj = -1;
	gchar* phsh = NULL;
	gchar* status = NULL;
	gchar* email = NULL;
	gchar* last = NULL;
	gchar* jid = NULL;
	gchar* AIM = NULL;
	gchar* vc = NULL;
	gchar* msg = NULL;
	gint address_length = 16;
	gchar* ip = NULL;
	sw_text_record_iterator iterator;
	sw_int8 key[SW_TEXT_RECORD_MAX_LEN];
	sw_int8 value[SW_TEXT_RECORD_MAX_LEN];
	sw_uint32 value_length;

gaim_debug_info("bonjour", "_resolve_reply\n");
	sw_discovery_cancel(discovery, oid);
	
	// Get the ip as a string
	ip = malloc(address_length);
	sw_ipv4_address_name(address, ip, address_length);

	// Obtain the parameters from the text_record
	if ((text_record_len > 0) && (text_record) && (*text_record != '\0')) {
		sw_text_record_iterator_init(&iterator, text_record, text_record_len);
		while (sw_text_record_iterator_next(iterator, key, value, &value_length) == SW_OKAY) {
			// Compare the keys with the possible ones and save them on 
			// the appropiate place of the buddy_list
			if (strcmp(key, "txtvers") == 0) {
				txtvers = g_strdup(value);
			} else if (strcmp(key, "version") == 0) {
				version = g_strdup(value);
			} else if (strcmp(key, "1st") == 0) {
				first = g_strdup(value);
			} else if (strcmp(key, "port.p2pj") == 0) {
				port_p2pj = atoi(value);
			} else if (strcmp(key, "status") == 0) {
				status = g_strdup(value);
			} else if (strcmp(key, "email") == 0) {
				email = g_strdup(value);
			} else if (strcmp(key, "last") == 0) {
				last = g_strdup(value);
			} else if (strcmp(key, "jid") == 0) {
				jid = g_strdup(value);
			} else if (strcmp(key, "AIM") == 0) {
				AIM = g_strdup(value);
			} else if (strcmp(key, "vc") == 0) {
				vc = g_strdup(value);
			} else if (strcmp(key, "phsh") == 0) {
				phsh = g_strdup(value);
			} else if (strcmp(key, "msg") == 0) {
				msg = g_strdup(value);
			}
		}
	}

	// Put the parameters of the text_record in a buddy and add the buddy to 
	// the buddy list
	buddy = bonjour_buddy_new((gchar*)name, first, port_p2pj, phsh, status, email,
		last, jid, AIM, vc, ip, msg);
		
	if (bonjour_buddy_check(buddy) == FALSE) {
		return SW_DISCOVERY_E_UNKNOWN;
	}
	
	// Look for the buddy within the buddy list, if exist, change the status information, 
	// else create a new buddy within the group Bonjour
	bonjour_buddy_add_to_gaim(buddy, account);
	
	// Free all the temporal strings
	g_free(txtvers);
	g_free(version);
	g_free(first);
	g_free(last);
	g_free(status);
	g_free(email);
	g_free(jid);
	g_free(AIM);
	g_free(vc);
	g_free(phsh);
	g_free(msg);
	
	return SW_OKAY;
}

static sw_result HOWL_API _browser_reply(sw_discovery discovery, sw_discovery_oid oid, 
			sw_discovery_browse_status status, sw_uint32 interface_index, sw_const_string name, 
			sw_const_string type, sw_const_string domain, sw_opaque_t extra)
{
	sw_discovery_resolve_id rid;
	GaimAccount* account = (GaimAccount*)extra;
	GaimBuddy* gb = NULL;
	
	switch(status){
		case SW_DISCOVERY_BROWSE_INVALID:
			gaim_debug_warning("bonjour", "_browser_reply --> Invalid\n");
			break;
		case SW_DISCOVERY_BROWSE_RELEASE:
			gaim_debug_warning("bonjour", "_browser_reply --> Release\n");
			break;
		case SW_DISCOVERY_BROWSE_ADD_DOMAIN:
			gaim_debug_warning("bonjour", "_browser_reply --> Add domain\n");
			break;
		case SW_DISCOVERY_BROWSE_ADD_DEFAULT_DOMAIN:
			gaim_debug_warning("bonjour", "_browser_reply --> Add default domain\n");
			break;
		case SW_DISCOVERY_BROWSE_REMOVE_DOMAIN:
			gaim_debug_warning("bonjour", "_browser_reply --> Remove domain\n");
			break;
		case SW_DISCOVERY_BROWSE_ADD_SERVICE:
			// A new peer has join the network and uses iChat bonjour
			gaim_debug_info("bonjour", "_browser_reply --> Add service\n");
			if (g_ascii_strcasecmp(name, account->username) != 0){
				if (sw_discovery_resolve(discovery, interface_index, name, type, 
						domain, _resolve_reply, extra, &rid) != SW_OKAY) {
					gaim_debug_warning("bonjour", "_browser_reply --> Cannot send resolve\n");
				}
			}
			break;
		case SW_DISCOVERY_BROWSE_REMOVE_SERVICE:
			gaim_debug_info("bonjour", "_browser_reply --> Remove service\n");
			gb = gaim_find_buddy((GaimAccount*)extra, name);
			if (gb != NULL) {
				bonjour_buddy_delete(gb->proto_data);
				gaim_blist_remove_buddy(gb);
			}
			break;
		case SW_DISCOVERY_BROWSE_RESOLVED:
			gaim_debug_info("bonjour", "_browse_reply --> Resolved\n");
			break;
		default:
			break;
	}
	
	return SW_OKAY;
}

dns_sd_packet* _dns_sd_packet_new()
{
	return g_new(dns_sd_packet, 1);
}

void _dns_sd_send_packet(dns_sd_packet* packet, sw_discovery* session)
{
	sw_text_record dns_data;
	sw_discovery_publish_id session_id;
gaim_debug_warning("bonjour", "dns_sd send packet --> Fill data for the service\n");	
	// Fill the data for the service	
	if(sw_text_record_init(&dns_data) != SW_OKAY){
		gaim_debug_error("bonjour", "Unable to initialize the data for the mDNS.");
		return;
	}
	
	sw_text_record_add_key_and_string_value(dns_data, "txtvers", packet->txtvers);
	sw_text_record_add_key_and_string_value(dns_data, "version", packet->version);
	sw_text_record_add_key_and_string_value(dns_data, "1st", packet->first);
	sw_text_record_add_key_and_string_value(dns_data, "last", packet->last);
	// sw_text_record_add_key_and_string_value(dns_data, "port.p2pj", itoa(packet->port_p2pj));
	sw_text_record_add_key_and_string_value(dns_data, "port.p2pj", BONJOUR_DEFAULT_PORT);
	sw_text_record_add_key_and_string_value(dns_data, "phsh", packet->phsh);
	sw_text_record_add_key_and_string_value(dns_data, "status", packet->status);
	sw_text_record_add_key_and_string_value(dns_data, "msg", packet->message);
	sw_text_record_add_key_and_string_value(dns_data, "email", packet->email);
	sw_text_record_add_key_and_string_value(dns_data, "vc", packet->vc);
	sw_text_record_add_key_and_string_value(dns_data, "jid", packet->jid);
	sw_text_record_add_key_and_string_value(dns_data, "AIM", packet->AIM);

	// Publish the service
	if(sw_discovery_publish(*session, 0, packet->name, ICHAT_SERVICE, NULL, 
			NULL, packet->port_p2pj, sw_text_record_bytes(dns_data), sw_text_record_len(dns_data), 
			_publish_reply, NULL, &session_id) != SW_OKAY){
		gaim_debug_error("bonjour", "Unable to publish or change the status of the _presence._tcp service.");
		return;
	}
	
	// Free the memory used by temp data
	sw_text_record_fina(dns_data);
}

void _dns_sd_packet_free(dns_sd_packet* packet)
{
	g_free(packet);
}

int _dns_sd_register(BonjourDnsSd* data)
{
	dns_sd_packet* packet;
	
	// Create a new dns-sd packet
	packet = _dns_sd_packet_new();
	packet->name = data->name;
	packet->txtvers = data->txtvers;
	packet->version = data->version;
	packet->first = data->first;
	packet->last = data->last;
	packet->port_p2pj = data->port_p2pj;
	packet->phsh = data->phsh;
	packet->status = data->status;
	packet->message = "";
	packet->email = data->email;
	packet->vc = data->vc;
	packet->jid = data->jid;
	packet->AIM = data->AIM;
	
	// Send the dns-sd packet
	_dns_sd_send_packet(packet, data->session);
	
	// Free the dns-sd packet
	_dns_sd_packet_free(packet);
	
	return 0;
}

gboolean _dns_sd_handle_packets(GIOChannel* source, GIOCondition condition, gpointer data)
{
	sw_discovery_read_socket(*((sw_discovery*)data));
	return TRUE;
}

gpointer _dns_sd_wait_for_connections(gpointer data)
{
	sw_discovery_oid session_id;
	BonjourDnsSd* dns_sd_data = (BonjourDnsSd*)data;
gaim_debug_error("bonjour", "Wait for connections\n");	
	// Advise the daemon that we are waiting for connections
	if(sw_discovery_browse(*(dns_sd_data->session), 0, ICHAT_SERVICE, NULL, _browser_reply,
			dns_sd_data->account, &session_id) != SW_OKAY){
		gaim_debug_error("bonjour", "Unable to get service.");
		return NULL;
	}

	// Yields control of the cpu to the daemon
	sw_discovery_run(*(dns_sd_data->session));
	
	return NULL;
}

// End private functions

/**
 * Allocate space for the dns-sd data.
 */
BonjourDnsSd* bonjour_dns_sd_new()
{
	BonjourDnsSd* data = g_new(BonjourDnsSd, 1);
	data->session = g_malloc(sizeof(sw_discovery));
	
	return data;
}

/**
 * Deallocate the space of the dns-sd data.
 */
void bonjour_dns_sd_free(BonjourDnsSd* data)
{
	g_free(data->session);
	g_free(data);
}

/**
 * Send a new dns-sd packet updating our status.
 */
void bonjour_dns_sd_send_status(BonjourDnsSd* data, char* status_dns_sd, const char* status_message)
{
	dns_sd_packet* packet;
	
	// Create a new dns-sd packet
	packet = _dns_sd_packet_new();
	packet->name = data->name;
	packet->txtvers = data->txtvers;
	packet->version = data->version;
	packet->first = data->first;
	packet->last = data->last;
	packet->port_p2pj = data->port_p2pj;
	packet->phsh = data->phsh;
	packet->status = status_dns_sd;
	packet->message = g_strdup(status_message);
	packet->email = data->email;
	packet->vc = data->vc;
	packet->jid = data->jid;
	packet->AIM = data->AIM;
	
	// Send the dns-sd packet
	_dns_sd_send_packet(packet, data->session);
	
	// Free the dns-sd packet
	_dns_sd_packet_free(packet);
}

/**
 * Advertise our presence within the dns-sd daemon and start browsing for other 
 * bonjour peers.
 */
void bonjour_dns_sd_start(BonjourDnsSd* data)
{	
	GIOChannel* io_channel;
	gint dns_sd_socket;
	sw_discovery_oid session_id;
	
	// Initilizations of the dns-sd data and session
	data->session = malloc(sizeof(sw_discovery));
	if(sw_discovery_init(data->session) != SW_OKAY){
			gaim_debug_error("bonjour", "Unable to initialize a mDNS session.");
			return;
	}
	
	// Register our bonjour IM client at the mDNS daemon
	_dns_sd_register(data);
	
	// Wait for new bonjour IM client connections, we should start a new thread
	/*
	if (!g_thread_supported ()){
		g_thread_init (NULL);
	}
	g_thread_create(_dns_sd_wait_for_connections, data, FALSE, NULL);
	*/
	
	// Advise the daemon that we are waiting for connections
	if(sw_discovery_browse(*(data->session), 0, ICHAT_SERVICE, NULL, _browser_reply,
			data->account, &session_id) != SW_OKAY){
		gaim_debug_error("bonjour", "Unable to get service.");
		return;
	}

	// Yields control of the cpu to the daemon
	//sw_discovery_run(*(dns_sd_data->session));
	
	// Get the socket that communicates with the mDNS daemon and binf it to a 
	// callback that will handle the dns_sd packets
	dns_sd_socket = sw_discovery_socket(*(data->session));
	io_channel = g_io_channel_unix_new(dns_sd_socket);
	g_io_add_watch(io_channel, G_IO_IN, _dns_sd_handle_packets, data->session); // Add more for other conditions like when the conn. has been broken
}

/**
 * Unregister the "_presence._tcp" service at the mDNS daemon.
 */
int bonjour_dns_sd_stop(BonjourDnsSd* data)
{
	sw_discovery_stop_run(*(data->session));
	
	return 0;
}
