/* PurpleWrapper - A .NET (CLR) wrapper for libpurple
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
 * clean interface between .NET/CLR and the unmanaged C library libpurple.
 *
 * This is the second major commit of the code.
 * Next things:
 *  - A few of the .h files have anonymous parameter names (eg: void cat(int, int).
 *    This program will need to assign these parameters names.
 *  - Function pointers inside structs aren't translated correctly into C#.
 *  - Two places there are specific-length arrays (eg: char hostname[256]). The parser
 *    does not detect them as an array.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class Certificate
	{
		public enum PurpleCertificateVerificationStatus
		{
			PURPLE_CERTIFICATE_INVALID = 0,
			PURPLE_CERTIFICATE_VALID = 1
		};

		/*
		 * PurpleCertificate * purple_certificate_copy(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_copy(IntPtr crt);

		public static PurpleCertificate Copy(PurpleCertificate crt)
		{
			return new PurpleCertificate(purple_certificate_copy(crt.Reference));
		}

		/*
		 * GList * purple_certificate_copy_list(GList * crt_list)
		 * 
		 * Could not generate a wrapper for purple_certificate_copy_list in file "certificate.h".
		 * Message: The type could not be resolved (GList * purple_certificate_copy_list(GList * crt_list)).
		 */

		/*
		 * void purple_certificate_destroy(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_destroy(IntPtr crt);

		public static void Destroy(PurpleCertificate crt)
		{
			purple_certificate_destroy(crt.Reference);
		}

		/*
		 * void purple_certificate_destroy_list(GList * crt_list)
		 * 
		 * Could not generate a wrapper for purple_certificate_destroy_list in file "certificate.h".
		 * Message: The type could not be resolved (GList * crt_list).
		 */

		/*
		 * gboolean purple_certificate_signed_by(PurpleCertificate * crt, PurpleCertificate * issuer)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_signed_by(IntPtr crt, IntPtr issuer);

		public static bool SignedBy(PurpleCertificate crt, PurpleCertificate issuer)
		{
			return purple_certificate_signed_by(crt.Reference, issuer.Reference);
		}

		/*
		 * gboolean purple_certificate_check_signature_chain(GList * chain)
		 * 
		 * Could not generate a wrapper for purple_certificate_check_signature_chain in file "certificate.h".
		 * Message: The type could not be resolved (GList * chain).
		 */

		/*
		 * PurpleCertificate * purple_certificate_import(PurpleCertificateScheme * scheme, gchar * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_import(IntPtr scheme, string filename);

		public static PurpleCertificate Import(PurpleCertificateScheme scheme, string filename)
		{
			return new PurpleCertificate(purple_certificate_import(scheme.Reference, filename));
		}

		/*
		 * gboolean purple_certificate_export(gchar * filename, PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_export(string filename, IntPtr crt);

		public static bool Export(string filename, PurpleCertificate crt)
		{
			return purple_certificate_export(filename, crt.Reference);
		}

		/*
		 * GByteArray * purple_certificate_get_fingerprint_sha1(PurpleCertificate * crt)
		 * 
		 * Could not generate a wrapper for purple_certificate_get_fingerprint_sha1 in file "certificate.h".
		 * Message: The type could not be resolved (GByteArray * purple_certificate_get_fingerprint_sha1(PurpleCertificate * crt)).
		 */

		/*
		 * gchar * purple_certificate_get_unique_id(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_certificate_get_unique_id(IntPtr crt);

		public static string GetUniqueId(PurpleCertificate crt)
		{
			return purple_certificate_get_unique_id(crt.Reference);
		}

		/*
		 * gchar * purple_certificate_get_issuer_unique_id(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_certificate_get_issuer_unique_id(IntPtr crt);

		public static string GetIssuerUniqueId(PurpleCertificate crt)
		{
			return purple_certificate_get_issuer_unique_id(crt.Reference);
		}

		/*
		 * gchar * purple_certificate_get_subject_name(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_certificate_get_subject_name(IntPtr crt);

		public static string GetSubjectName(PurpleCertificate crt)
		{
			return purple_certificate_get_subject_name(crt.Reference);
		}

		/*
		 * gboolean purple_certificate_check_subject_name(PurpleCertificate * crt, gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_check_subject_name(IntPtr crt, string name);

		public static bool CheckSubjectName(PurpleCertificate crt, string name)
		{
			return purple_certificate_check_subject_name(crt.Reference, name);
		}

		/*
		 * gboolean purple_certificate_get_times(PurpleCertificate * crt, time_t * activation, time_t * expiration)
		 * 
		 * Could not generate a wrapper for purple_certificate_get_times in file "certificate.h".
		 * Message: The type could not be resolved (time_t * activation).
		 */

		/*
		 * gchar * purple_certificate_pool_mkpath(PurpleCertificatePool * pool, gchar * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_certificate_pool_mkpath(IntPtr pool, string id);

		public static string PoolMkpath(PurpleCertificatePool pool, string id)
		{
			return purple_certificate_pool_mkpath(pool.Reference, id);
		}

		/*
		 * gboolean purple_certificate_pool_usable(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_usable(IntPtr pool);

		public static bool PoolUsable(PurpleCertificatePool pool)
		{
			return purple_certificate_pool_usable(pool.Reference);
		}

		/*
		 * PurpleCertificateScheme * purple_certificate_pool_get_scheme(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_pool_get_scheme(IntPtr pool);

		public static PurpleCertificateScheme PoolGetScheme(PurpleCertificatePool pool)
		{
			return new PurpleCertificateScheme(purple_certificate_pool_get_scheme(pool.Reference));
		}

		/*
		 * gboolean purple_certificate_pool_contains(PurpleCertificatePool * pool, gchar * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_contains(IntPtr pool, string id);

		public static bool PoolContains(PurpleCertificatePool pool, string id)
		{
			return purple_certificate_pool_contains(pool.Reference, id);
		}

		/*
		 * PurpleCertificate * purple_certificate_pool_retrieve(PurpleCertificatePool * pool, gchar * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_pool_retrieve(IntPtr pool, string id);

		public static PurpleCertificate PoolRetrieve(PurpleCertificatePool pool, string id)
		{
			return new PurpleCertificate(purple_certificate_pool_retrieve(pool.Reference, id));
		}

		/*
		 * gboolean purple_certificate_pool_store(PurpleCertificatePool * pool, gchar * id, PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_store(IntPtr pool, string id, IntPtr crt);

		public static bool PoolStore(PurpleCertificatePool pool, string id, PurpleCertificate crt)
		{
			return purple_certificate_pool_store(pool.Reference, id, crt.Reference);
		}

		/*
		 * gboolean purple_certificate_pool_delete(PurpleCertificatePool * pool, gchar * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_pool_delete(IntPtr pool, string id);

		public static bool PoolDelete(PurpleCertificatePool pool, string id)
		{
			return purple_certificate_pool_delete(pool.Reference, id);
		}

		/*
		 * GList * purple_certificate_pool_get_idlist(PurpleCertificatePool * pool)
		 * 
		 * Could not generate a wrapper for purple_certificate_pool_get_idlist in file "certificate.h".
		 * Message: The type could not be resolved (GList * purple_certificate_pool_get_idlist(PurpleCertificatePool * pool)).
		 */

		/*
		 * void purple_certificate_pool_destroy_idlist(GList * idlist)
		 * 
		 * Could not generate a wrapper for purple_certificate_pool_destroy_idlist in file "certificate.h".
		 * Message: The type could not be resolved (GList * idlist).
		 */

		/*
		 * void purple_certificate_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_init();

		public static void Init()
		{
			purple_certificate_init();
		}

		/*
		 * void purple_certificate_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_uninit();

		public static void Uninit()
		{
			purple_certificate_uninit();
		}

		/*
		 * gpointer purple_certificate_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_certificate_get_handle();
		}

		/*
		 * PurpleCertificateScheme * purple_certificate_find_scheme(gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_find_scheme(string name);

		public static PurpleCertificateScheme FindScheme(string name)
		{
			return new PurpleCertificateScheme(purple_certificate_find_scheme(name));
		}

		/*
		 * GList * purple_certificate_get_schemes()
		 * 
		 * Could not generate a wrapper for purple_certificate_get_schemes in file "certificate.h".
		 * Message: The type could not be resolved (GList * purple_certificate_get_schemes()).
		 */

		/*
		 * gboolean purple_certificate_register_scheme(PurpleCertificateScheme * scheme)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_register_scheme(IntPtr scheme);

		public static bool RegisterScheme(PurpleCertificateScheme scheme)
		{
			return purple_certificate_register_scheme(scheme.Reference);
		}

		/*
		 * gboolean purple_certificate_unregister_scheme(PurpleCertificateScheme * scheme)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_unregister_scheme(IntPtr scheme);

		public static bool UnregisterScheme(PurpleCertificateScheme scheme)
		{
			return purple_certificate_unregister_scheme(scheme.Reference);
		}

		/*
		 * PurpleCertificateVerifier * purple_certificate_find_verifier(gchar * scheme_name, gchar * ver_name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_find_verifier(string scheme_name, string ver_name);

		public static PurpleCertificateVerifier FindVerifier(string scheme_name, string ver_name)
		{
			return new PurpleCertificateVerifier(purple_certificate_find_verifier(scheme_name, ver_name));
		}

		/*
		 * GList * purple_certificate_get_verifiers()
		 * 
		 * Could not generate a wrapper for purple_certificate_get_verifiers in file "certificate.h".
		 * Message: The type could not be resolved (GList * purple_certificate_get_verifiers()).
		 */

		/*
		 * gboolean purple_certificate_register_verifier(PurpleCertificateVerifier * vr)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_register_verifier(IntPtr vr);

		public static bool RegisterVerifier(PurpleCertificateVerifier vr)
		{
			return purple_certificate_register_verifier(vr.Reference);
		}

		/*
		 * gboolean purple_certificate_unregister_verifier(PurpleCertificateVerifier * vr)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_unregister_verifier(IntPtr vr);

		public static bool UnregisterVerifier(PurpleCertificateVerifier vr)
		{
			return purple_certificate_unregister_verifier(vr.Reference);
		}

		/*
		 * PurpleCertificatePool * purple_certificate_find_pool(gchar * scheme_name, gchar * pool_name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_certificate_find_pool(string scheme_name, string pool_name);

		public static PurpleCertificatePool FindPool(string scheme_name, string pool_name)
		{
			return new PurpleCertificatePool(purple_certificate_find_pool(scheme_name, pool_name));
		}

		/*
		 * GList * purple_certificate_get_pools()
		 * 
		 * Could not generate a wrapper for purple_certificate_get_pools in file "certificate.h".
		 * Message: The type could not be resolved (GList * purple_certificate_get_pools()).
		 */

		/*
		 * gboolean purple_certificate_register_pool(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_register_pool(IntPtr pool);

		public static bool RegisterPool(PurpleCertificatePool pool)
		{
			return purple_certificate_register_pool(pool.Reference);
		}

		/*
		 * gboolean purple_certificate_unregister_pool(PurpleCertificatePool * pool)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_certificate_unregister_pool(IntPtr pool);

		public static bool UnregisterPool(PurpleCertificatePool pool)
		{
			return purple_certificate_unregister_pool(pool.Reference);
		}

		/*
		 * void purple_certificate_display_x509(PurpleCertificate * crt)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_display_x509(IntPtr crt);

		public static void DisplayX509(PurpleCertificate crt)
		{
			purple_certificate_display_x509(crt.Reference);
		}

		/*
		 * void purple_certificate_add_ca_search_path(char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_certificate_add_ca_search_path(string path);

		public static void AddCaSearchPath(string path)
		{
			purple_certificate_add_ca_search_path(path);
		}

	}
}

