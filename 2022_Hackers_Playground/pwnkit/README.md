# Pwnkit (CVE-2021-4034)

Alert! It is reported that a classic 0-day vulnerability,
aka [CVE-2021-4034](https://nvd.nist.gov/vuln/detail/CVE-2021-4034),
was found in a setuid binary, called [polkit](https://gitlab.freedesktop.org/polkit/polkit).
Soon after, numerous PoC exploits are developed and spread all over the
Internet (try search!). But no worry, our security response team just deployed
a strong mitigation against these exploitation, long time before
the OSS community came up with [a patch](https://gitlab.freedesktop.org/polkit/polkit/-/commit/a2bf5c9c83b6ae46cbd5c779d3055bff81ded683)! And our patch
seems to prevent all the known PoC on the Internet:

```diff
diff --git a/src/programs/pkexec.c b/src/programs/pkexec.c
index 7698c5c..bc6305e 100644
--- a/src/programs/pkexec.c
+++ b/src/programs/pkexec.c
@@ -505,12 +505,17 @@ main (int argc, char *argv[])
   setenv ("GIO_USE_VFS", "local", 1);
 
   /* check for correct invocation */
-  if (geteuid () != 0)
+  if (geteuid () == getuid())
     {
-      g_printerr ("pkexec must be setuid root\n");
+      g_printerr ("pkexec must be setuid binary\n");
       goto out;
     }
 
+  /* CVE-2021-4034 mitigation: clean up PATH for the setuid-binary */
+  if (getenv("PATH") != NULL) {
+    setenv("PATH", "/bin:/usr/bin", 1);
+  }
+
   original_user_name = g_strdup (g_get_user_name ());
   if (original_user_name == NULL)
     {
```

Ah, we didn't forget to deploy the ["latest" glib](https://github.com/GNOME/glib/commit/fe958652677150d98086f148ac748557f223a4c0)
along with this patch!

    $ ssh sstf@[**SSTF-SERVER**] -p 8042
    sstf@localhost's password: [sstf]
    $ ls -l /home/pwnkit/
    total 100
    -rw-rw-r-- 1 root   root       3 Mar 24 07:13 README
    -r-------- 1 pwnkit pwnkit    30 Mar 24 07:03 flag
    -rwsrwsr-x 1 pwnkit pwnkit 90752 Mar 24 05:41 pkexec

Get the `flag` by exploiting `pkexec`!

## Hint.

- Feel free to try out known PoCs, but don't forget to learn its root cause
  and exploitation trick!
- Use `/tmp/[top-secret]/` but with caution


## Solving Strategy
- [PwnKit](https://github.com/ly4k/PwnKit) is a known PoC of CVE-2021-4034.
  PwnKit uses `PATH` environment variable, which is sanitized in the patch of this challenge.
  Thus, we should understand how PwnKit exploits the bug and modify PwnKit to bypass the patch.

- This challenge uses the "latest" glib, so [these](https://github.com/GNOME/glib/commit/78dc1cc3cd669e9fb92ae7847bab2b308c0a8628) [commits](https://github.com/GNOME/glib/commit/dba1fce993b280e2c9481a0d94ec2cc0ddc26739) are applied to it.
  According to the commits, `g_find_program_in_path()` uses `g_get_current_dir()` to find absolute path of the given program.
  Then, `g_get_current_dir()` may return the value of `PWD` environment variable as the current dir.
  As a result, we can use `PWD` environment variable instead of `PATH` for PwnKit!

- Modify PwnKit to use `PWD` environment!

