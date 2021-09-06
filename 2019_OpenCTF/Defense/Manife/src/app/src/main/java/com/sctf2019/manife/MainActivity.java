package com.sctf2019.manife;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;

/**
 * @author vkarko
 * @since 30.05.19 18:21
 */
public class MainActivity extends AppCompatActivity implements ServiceConnection  {
    private static final String TAG = MainActivity.class.getSimpleName();
    private IAttestationService attestationService;

    @Override
    protected void onStart() {
        super.onStart();

        if (!Features.checkSdkVersions(this) ||
            !Features.checkComponents(this) ||
            !Features.checkMainIntentFilter(getIntent()) ||
            !Features.checkPermissions(this)) {
            finish();
            return;
        }

        final Intent intent = new Intent(AttestationService.START_ACTION);
        intent.setClass(this, AttestationService.class);
        bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        attestationService = IAttestationService.Stub.asInterface(service);

        final String deviceId = Features.getDeviceId(this);
        Log.d(TAG, "getDeviceId: " + deviceId);

        try {
            if (!attestationService.attest(deviceId)) {
                finish();
                return;
            }
        } catch (RemoteException e) {
            throw new RuntimeException(e);
        }

        final Bundle result = getContentResolver().call(Uri.parse("content://com.sctf2019.manife/"), "hash", null, null);
        if (result == null) return;

        final String hash = result.getString(HashProvider.RESULT);
        if (hash == null) return;

//        final String x = CryptoUtils.toHex(CryptoUtils.xor(CryptoUtils.toBytes(hash), "SCTF{m4n1f357_15_r3c0n57ruc73d!}".getBytes()));
//         <string name="flag">b5187ac530cd874581352cade4a270fdfb26190799d8492c55703a82a7036d42</string>

        final String flag = new String(CryptoUtils.xor(CryptoUtils.toBytes(hash), CryptoUtils.toBytes(getString(R.string.flag))));
        Log.w(TAG, flag);
        Toast.makeText(this, flag, Toast.LENGTH_LONG).show();
    }

    @Override
    protected void onStop() {
        super.onStop();
        unbindService(this);
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        attestationService = null;
    }
}
