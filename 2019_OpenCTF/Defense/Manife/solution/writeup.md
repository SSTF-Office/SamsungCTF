Manife… Writeup 
===
###### Target:
> 577b2d9df16a8c7c8e79c364e4b9323f  manife.apk

###### Exploitation:
Given apk file is invalid due to missing `AndroidManifest.xml`, so we cannot install it and need to use static analysis.
Decompile apk (e.g. JADx https://github.com/skylot/jadx) and start investigation.

1. Analyze decompiled code

    There are `MainActivity` which might be an entry point. On its `onStart` method there are sequence of some checks
    (`Features.checkSdkVersions()`, `Features.checkComponents`, etc.) then activity binds to service `AttestationService`.
    
    When service is connected (`onServiceConnected`) application attest device ID, 
    then make a call to content provider that initialize `HashBuilder` and compute some hash, 
    then _xor_ it with hardcoded value `R.string.flag` in string resources and, 
    finally, popup notification and print result to log.
    That might be a flag.

2. Hashing scheme

    `HashBuilder.build()` method creates a tree structure from compiled `AndroidManifest.xml` retrieved from its own apk, 
    where each tree node contain not excluded tags or attributes with values from the manifest. 
    
    Then it traverses the tree starting from the leaves up to the root (Depth First Traversal order), 
    for each node computes the cryptographic hash of node's value concatenated with xor of all children nodes.
    <pre>
    n&#7522; = HASH(n&#7522; &#8741; n&#7522;&#8321; &oplus; n&#7522;&#8322; &oplus; … &oplus; n&#7522;&#8342;)
    </pre>
    
    In that way, order of nodes does not affect root's hash.
    
    So far, we need to reconstruct original manifest.

3. Manifest reconstruction

    Analyze decompiled code more to collect hints and evidences about missing manifest parts:
    
    * `BuildConfig` class contains info about package name, version code and name:
        ```java
        public static final String APPLICATION_ID = "com.sctf2019.manife";
        public static final int VERSION_CODE = 137;
        public static final String VERSION_NAME = "1.3.7-00-13371-global";
        ...
        ```
    
    * `Features` class contains info about:
        *  minimal sdk version `28`, which code name is `9`
            ```java
            @RequiresApi(api = 28)
            public static String getDeviceId(Context context) { .. }
            ```
            
        * required permission `android.permission.READ_PHONE_STATE`
            ```java
            TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            String deviceId = telephonyManager.getDeviceId();
            ```
            
        * `checkSdkVersions()` reveals that `minSdkVersion` = `targetSdkVersion` = `compileSdkVersion` = `platformBuildVersionCode`
            ```java
            int i = Build.VERSION.SDK_INT;
            if (i == packageInfo.applicationInfo.minSdkVersion && i == packageInfo.applicationInfo.targetSdkVersion) {
                return true;
            }
            ```
            
        * `checkPermissions()` reveals that there should be defined and used permissions with `signature` level, 
        while `Manifest` class reveals theirs names. According to the names, they are have to be applied to the content provider.
        
        * `checkComponents()` reveals that application component should be enabled
            ```java
            if (context.getPackageManager().getApplicationInfo(context.getPackageName(), 0).enabled) {
                return true;
            }
            ```        
            and there are exact one service component, one provider and one activity. 
            All of them should be enabled and exported.
            
        * `checkMainIntentFilter()` reveals that activity component should contain intent filter with action `android.intent.action.MAIN` and 
        category `android.intent.category.LAUNCHER`
        
    * `MainActivity` class contains info about:
        * content provider authority:
            ```java
            Bundle call = getContentResolver().call(Uri.parse("content://com.sctf2019.manife/"), "hash", null, null);
            ```
        
        * service intent filter action `com.sctf2019.manife.service.START_ACTION`
            ```java
            public void onStart() {
                ...
                Intent intent = new Intent(AttestationService.START_ACTION);
                intent.setClass(this, AttestationService.class);
                bindService(intent, this, 1);
                return;
            }
            ```
            
    * Empty class `ManifeFactory` extended from `android.app.AppComponentFactory` class, has to be used to override default factory:
        ```xml
        <application
            android:appComponentFactory="com.sctf2019.manife.ManifeFactory"
            ...
        ```
        
    * Application's icons, label and theme ID are likely to be standard. 
    However, it is better to to check IDs from `values/public.xml` file which contains mapping of resources IDs to their names.
    
    * Note, that `HashProvider` setup some attributes that are not involved in hash calculation.
    
4. The Final Cut

    At last, we need to analyze `AttestationService`, that uses Android Binder IPC which remote part is implemented in anonymous class. 
    Method `attest` retrieves metadata from application
    ```java
    str = packageManager.getApplicationInfo(packagesForUid[0], 128).metaData.getString(AttestationService.API_SECRET_KEY);
    ```
    and then apply a series of checks, so we could recover value except 2 bytes.
    
    Since we have collected all available info about missing manifest pieces we could apply brute-force (2 bytes = 65536 combinations in the worst case scenario).
    
    Create dummy project in Android Studio, copy `HashBuilder` and all dependent classes into project.
    Prepare reconstructed manifest according to previously collected info:
    ```xml
    <?xml version="1.0" encoding="utf-8"?>
    <manifest
        xmlns:android="http://schemas.android.com/apk/res/android"
        android:versionCode="137"
        android:versionName="1.3.7-00-13371-global"
        android:compileSdkVersion="28"
        android:compileSdkVersionCodename="9"
        package="com.sctf2019.manife"
        platformBuildVersionCode="28"
        platformBuildVersionName="9">
    
        <permission
            android:protectionLevel="signature"
            android:name="com.sctf2019.manife.provider.READ_PERMISSION" />
    
        <permission
            android:protectionLevel="signature"
            android:name="com.sctf2019.manife.provider.WRITE_PERMISSION" />
    
        <uses-permission android:name="com.sctf2019.manife.provider.READ_PERMISSION" />
        <uses-permission android:name="com.sctf2019.manife.provider.WRITE_PERMISSION" />
        <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    
        <uses-sdk
            android:minSdkVersion="28"
            android:targetSdkVersion="28" />
    
        <application
            android:icon="@mipmap/ic_launcher"
            android:label="@string/app_name"
            android:roundIcon="@mipmap/ic_launcher_round"
            android:theme="@style/AppTheme"
            android:enabled="true"
            android:appComponentFactory="com.sctf2019.manife.ManifeFactory">
    
            <meta-data
                android:name="api_secret_key"
                android:value="a323eb1e-61b2-XXXX-b6f9-0a5c6ada7dec" />
    
            <activity
                android:enabled="true"
                android:exported="true"
                android:name="com.sctf2019.manife.MainActivity">
                <intent-filter>
                    <action android:name="android.intent.action.MAIN" />
                    <category android:name="android.intent.category.LAUNCHER" />
                </intent-filter>
            </activity>
    
            <service
                android:enabled="true"
                android:exported="true"
                android:name="com.sctf2019.manife.AttestationService">
                <intent-filter>
                    <action android:name="com.sctf2019.manife.service.START_ACTION" />
                </intent-filter>
            </service>
    
            <provider
                android:enabled="true"
                android:exported="true"
                android:authorities="com.sctf2019.manife"
                android:name="com.sctf2019.manife.HashProvider"
                android:readPermission="com.sctf2019.manife.provider.READ_PERMISSION"
                android:writePermission="com.sctf2019.manife.provider.WRITE_PERMISSION"/>
        </application>
    </manifest>
    ```
    Save it as `res/layout/base.xml` (location in `layout` folder ensures that `*xml` files will be compiled) 
    and change `HashBuilder.build()` method to read it instead of `AndroidManifest.xml`. Then apply brute-force.
    
    Full Exploit:
    ```java
    public class ExploitActivity extends AppCompatActivity {
        public static final byte[] SECRET = CryptoUtils.toBytes("b5187ac530cd874581352cade4a270fdfb26190799d8492c55703a82a7036d42");
    
        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main);
    
            try {
                final AssetManager assetManager = createPackageContext(getPackageName(), 0).getAssets();
                final MessageDigest digest = MessageDigest.getInstance("SHA-256");
                final HashBuilder builder = new HashBuilder(digest);
    
                HashNode root;
                for (int i = 0; i < 65536; i++) {
                    root = new HashNode();
    
                    try (XmlResourceParser parser = assetManager.openXmlResourceParser(0, "res/layout/base.xml")){
                        HashNode current = root;
                        for (int event = parser.getEventType(); event != 1; event = parser.nextToken()) {
                            if (event != 0) {
                                if (event == 2) {
                                    final String tag = parser.getName();
    
                                    current = new HashNode(tag, current);
                                    for (int j = 0; j < parser.getAttributeCount(); j++) {
                                        final String attribute = parser.getAttributeName(j);
    
                                        if ("meta-data".equals(tag) && "value".equals(attribute)) {
                                            final String apiKey = String.format("a323eb1e-61b2-%04x-b6f9-0a5c6ada7dec", i);
    
                                            new HashNode(attribute + "=" + apiKey, current);
                                        } else {
                                            new HashNode(attribute + "=" + parser.getAttributeValue(j), current);
                                        }
                                    }
                                } else if (event != 3) {
                                    throw new UnsupportedOperationException();
                                } else if (current != null) {
                                    current = current.getParent();
                                }
                            }
                        }
                    }
    
                    builder.accept(root);
    
                    final String flag = new String(CryptoUtils.xor(CryptoUtils.toBytes(root.value), SECRET));
                    Log.i("MainActivity", "i: " + i);
    
                    if (flag.startsWith("SCTF")) {
                        Log.w("FLAG", "FLAG: " + flag);
                        break;
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    ```
    
5. PROFIT!
    ```
    ...
    3491-3491/com.foo.dymmy I/MainActivity: i: 19047
    3491-3491/com.foo.dymmy I/MainActivity: i: 19048
    3491-3491/com.foo.dymmy I/MainActivity: i: 19049
    3491-3491/com.foo.dymmy I/MainActivity: i: 19050
    3491-3491/com.foo.dymmy W/FLAG: FLAG: SCTF{m4n1f357_15_r3c0n57ruc73d!}
    ```













