package com.sctf2019.manife;

import android.app.Service;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.IBinder;
import android.os.RemoteException;

import androidx.annotation.Nullable;

/**
 * @author vkarko
 * @since 03.06.19 14:06
 */
public class AttestationService extends Service {
    /**
     * <code>a323eb1e-61b2-<b>4a6a</b>-b6f9-0a5c6ada7decx</code>
     */
    public static final String API_SECRET_KEY = "api_secret_key";
    public static final String START_ACTION = "com.sctf2019.manife.service.START_ACTION";

    private final IBinder binder = new IAttestationService.Stub() {
        @Override
        public boolean attest(String deviceId) throws RemoteException {
            if (deviceId.equals("f120bb5698d520c5691b6d603a00bfd662d13bf177a04571f9d10c0745dfa2a5")) return false;
            if (deviceId.equals("14bdcd6fd64180af5e7791df91b6af8e9a3e7bc844997eb8c29252706df97ca5")) return false;

            final PackageManager packageManager = AttestationService.this.getPackageManager();
            final int callingUid = getCallingUid();

            final String[] packagesForUid = packageManager.getPackagesForUid(callingUid);
            if (packagesForUid == null || packagesForUid.length != 1) return false;

            try {
                final ApplicationInfo info = packageManager.getApplicationInfo(packagesForUid[0], PackageManager.GET_META_DATA);
                final String key = info.metaData.getString(API_SECRET_KEY);
                if (key == null) return false;

                final String[] split = key.split("-");
                if (split.length != 5) return false;

                if (split[0].length() != 8) return false;
                if (split[1].length() != split[2].length()) return false;
                if (split[2].length() != split[3].length()) return false;
                if (split[3].length() != 4) return false;
                if (split[4].length() != 12) return false;

                for (int i = 0; i < split.length; i++) {
                    final byte[] bytes = CryptoUtils.toBytes(split[i]);

                    switch (i) {
                        case 0: {
                            if (bytes[0] != (byte) 0xa3) return false;
                            if (bytes[1] != (byte) 0x23) return false;
                            if (bytes[2] != (byte) 0xeb) return false;
                            if (bytes[3] != (byte) 0x1e) return false;
                        } break;

                        case 1: {
                            if (bytes[0] != (byte) 0x61) return false;
                            if (bytes[1] != (byte) 0xb2) return false;
                        } break;

                        case 3: {
                            if (bytes[0] != (byte) 0xb6) return false;
                            if (bytes[1] != (byte) 0xf9) return false;
                        } break;

                        case 4: {
                            if (bytes[0] != (byte) 0x0a) return false;
                            if (bytes[1] != (byte) 0x5c) return false;
                            if (bytes[2] != (byte) 0x6a) return false;
                            if (bytes[3] != (byte) 0xda) return false;
                            if (bytes[4] != (byte) 0x7d) return false;
                            if (bytes[5] != (byte) 0xec) return false;
                        } break;
                    }
                }

                return true;
            } catch (PackageManager.NameNotFoundException e) {
                return false;
            }
        }
    };

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return START_ACTION.equals(intent.getAction()) ? binder : null;
    }
}
