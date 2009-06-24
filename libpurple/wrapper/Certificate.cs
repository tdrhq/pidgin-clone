/* purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

/*
 * This file was auto-generated from the libpurple header files to provide a
 * clean interface between .NET/CLR and the unmanaged C library, libpurple.
 *
 * This code isn't complete, but completely a work in progress. :)
 * Three major things left:
 *  - Resolve the remaining UNKNOWN types.
 *  - Handle translation between delegate and function pointers.
 *  - Fill in the translation between public .NET class calls and private DllImport[] calls.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class Certificate
	{
		/*
		 * void purple_certificate_verify(PurpleCertificateVerifier * verifier, gchar * subject_name, GList * cert_chain, PurpleCertificateVerifiedCallback cb, gpointer cb_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_verify(IntPtr verifier, string subject_name, IntPtr cert_chain, UNKNOWN cb, IntPtr cb_data);

		public static void Verify(PurpleCertificateVerifier verifier, string subject_name, GList cert_chain, PurpleCertificateVerifiedCallback cb, IntPtr cb_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_verify_complete(PurpleCertificateVerificationRequest * vrq, PurpleCertificateVerificationStatus st)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_verify_complete(IntPtr vrq, UNKNOWN st);

		public static void VerifyComplete(PurpleCertificateVerificationRequest vrq, PurpleCertificateVerificationStatus st)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_destroy(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_destroy(IntPtr crt);

		public static void Destroy(PurpleCertificate crt)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_destroy_list( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_destroy_list(UNKNOWN );

		public static void DestroyList( )
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_signed_by(PurpleCertificate * crt, PurpleCertificate * issuer)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_signed_by(IntPtr crt, IntPtr issuer);

		public static bool SignedBy(PurpleCertificate crt, PurpleCertificate issuer)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_check_signature_chain(GList * chain)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_check_signature_chain(IntPtr chain);

		public static bool CheckSignatureChain(GList chain)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_export(gchar * filename, PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_export(string filename, IntPtr crt);

		public static bool Export(string filename, PurpleCertificate crt)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_check_subject_name(PurpleCertificate * crt, gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_check_subject_name(IntPtr crt, string name);

		public static bool CheckSubjectName(PurpleCertificate crt, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_get_times(PurpleCertificate * crt, time_t * activation, time_t * expiration)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_get_times(IntPtr crt, IntPtr activation, IntPtr expiration);

		public static bool GetTimes(PurpleCertificate crt, time_t activation, time_t expiration)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_pool_usable(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_usable(IntPtr pool);

		public static bool PoolUsable(PurpleCertificatePool pool)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_pool_contains(PurpleCertificatePool * pool, gchar * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_contains(IntPtr pool, string id);

		public static bool PoolContains(PurpleCertificatePool pool, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_pool_store(PurpleCertificatePool * pool, gchar * id, PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_store(IntPtr pool, string id, IntPtr crt);

		public static bool PoolStore(PurpleCertificatePool pool, string id, PurpleCertificate crt)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_pool_delete(PurpleCertificatePool * pool, gchar * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_delete(IntPtr pool, string id);

		public static bool PoolDelete(PurpleCertificatePool pool, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_pool_destroy_idlist(GList * idlist)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_pool_destroy_idlist(IntPtr idlist);

		public static void PoolDestroyIdlist(GList idlist)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_certificate_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_register_scheme(PurpleCertificateScheme * scheme)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_register_scheme(IntPtr scheme);

		public static bool RegisterScheme(PurpleCertificateScheme scheme)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_unregister_scheme(PurpleCertificateScheme * scheme)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_unregister_scheme(IntPtr scheme);

		public static bool UnregisterScheme(PurpleCertificateScheme scheme)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_register_verifier(PurpleCertificateVerifier * vr)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_register_verifier(IntPtr vr);

		public static bool RegisterVerifier(PurpleCertificateVerifier vr)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_unregister_verifier(PurpleCertificateVerifier * vr)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_unregister_verifier(IntPtr vr);

		public static bool UnregisterVerifier(PurpleCertificateVerifier vr)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_register_pool(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_register_pool(IntPtr pool);

		public static bool RegisterPool(PurpleCertificatePool pool)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_certificate_unregister_pool(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_unregister_pool(IntPtr pool);

		public static bool UnregisterPool(PurpleCertificatePool pool)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_display_x509(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_display_x509(IntPtr crt);

		public static void DisplayX509(PurpleCertificate crt)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_certificate_add_ca_search_path(char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_add_ca_search_path(string path);

		public static void AddCaSearchPath(string path)
		{
			throw new NotImplementedException();
		}

	}
}

