package com.sctf2019.manife;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PermissionInfo;
import android.os.Build;
import android.provider.Settings;
import android.security.keystore.KeyGenParameterSpec;
import android.security.keystore.KeyProperties;
import android.telephony.TelephonyManager;

import androidx.annotation.RequiresApi;

import java.io.IOException;
import java.nio.charset.Charset;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;

import static android.content.pm.PermissionInfo.PROTECTION_SIGNATURE;

/**
 * @author vkarko
 * @since 31.05.19 9:37
 */
@SuppressLint("HardwareIds")
public class Features {

    // ToDo: minSdkVersion 28
    @RequiresApi(api = Build.VERSION_CODES.P)
    public static String getDeviceId(Context context) {
        String androidId = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
        byte[] bytes;

        try {
            final TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            if (telephonyManager == null) throw new AssertionError();

            // ToDo: <uses-permission android:name="android.permission.READ_PHONE_STATE" />
            final String deviceId = telephonyManager.getDeviceId();

            final KeyStore keyStore = KeyStore.getInstance("AndroidKeyStore");
            keyStore.load(null);

            if (!keyStore.containsAlias("secureId")) {
                final KeyGenParameterSpec keySpec = new KeyGenParameterSpec.Builder(
                        "secureId",
                        KeyProperties.PURPOSE_ENCRYPT | KeyProperties.PURPOSE_DECRYPT
                )
                .setBlockModes(KeyProperties.BLOCK_MODE_CBC)
                .setEncryptionPaddings(KeyProperties.ENCRYPTION_PADDING_NONE)
                // ToDo: minSdkVersion 28
                .setUnlockedDeviceRequired(true)
                .build();

                final KeyGenerator keyGenerator = KeyGenerator.getInstance(KeyProperties.KEY_ALGORITHM_AES, "AndroidKeyStore");
                keyGenerator.init(keySpec);
                keyGenerator.generateKey();
            }

            final KeyStore.SecretKeyEntry keyStoreEntry = (KeyStore.SecretKeyEntry) keyStore.getEntry("secureId", null);
            final SecretKey secretKey = keyStoreEntry.getSecretKey();

            final Cipher cipher = Cipher.getInstance("AES/CBC/NoPadding");
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            cipher.update(deviceId.getBytes(Charset.forName("UTF-8")));
            bytes = cipher.doFinal(androidId.getBytes(Charset.forName("UTF-8")));

        } catch (SecurityException | GeneralSecurityException | IOException | AssertionError e) {
            try {
                final MessageDigest digest = MessageDigest.getInstance("SHA-256");
                bytes = digest.digest(androidId.getBytes(Charset.forName("UTF-8")));

            } catch (NoSuchAlgorithmException e2) {
                throw new RuntimeException(e2);
            }
        }

        return CryptoUtils.toHex(bytes);
    }


    /**
     * android:minSdkVersion == android:targetSdkVersion == compileSdkVersion == platformBuildVersionCode
     *
     * @see <a href="https://stackoverflow.com/questions/39291397/whats-platformbuildversioncode-and-platformbuildversionname-in-extracted-apks">
     *     What's platformBuildVersionCode and platformBuildVersionName in Extracted Apk's</a>
     */
    public static boolean checkSdkVersions(Context context) {
        try {
            final PackageInfo info = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            final int version = Build.VERSION.SDK_INT; // = compileSdkVersion
            return version == info.applicationInfo.minSdkVersion &&
                    version == info.applicationInfo.targetSdkVersion;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }

    public static boolean checkPermissions(Context context) {
        try {
            final PackageInfo info = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_PERMISSIONS);
            if (info.permissions == null) return false;

            for (PermissionInfo permission : info.permissions) {
                if (permission.protectionLevel != PROTECTION_SIGNATURE) return false;
                if (context.checkSelfPermission(permission.name) != PackageManager.PERMISSION_GRANTED) return false;
            }

            return true;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }

    public static boolean checkComponents(Context context) {
        try {
            final ApplicationInfo applicationInfo = context.getPackageManager().getApplicationInfo(context.getPackageName(), 0);
            if (!applicationInfo.enabled) return false;

            final PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SERVICES | PackageManager.GET_PROVIDERS | PackageManager.GET_ACTIVITIES);
            if (packageInfo.services.length != 1 || !packageInfo.services[0].enabled || !packageInfo.services[0].exported) return false;
            if (packageInfo.providers.length != 1 || !packageInfo.providers[0].enabled || !packageInfo.providers[0].exported) return false;
            if (packageInfo.activities.length != 1 || !packageInfo.activities[0].enabled || !packageInfo.activities[0].exported) return false;
            return true;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }

    public static boolean checkMainIntentFilter(Intent intent) {
        return intent != null &&
                Intent.ACTION_MAIN.equals(intent.getAction()) &&
                intent.hasCategory(Intent.CATEGORY_LAUNCHER);
    }
}
