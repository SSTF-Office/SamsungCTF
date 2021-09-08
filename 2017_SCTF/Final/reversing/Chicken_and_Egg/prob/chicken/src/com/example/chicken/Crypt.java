package com.example.chicken;

import android.content.res.AssetManager;
import android.util.Log;
import dalvik.system.DexFile;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;

public class Crypt
{
    static { System.loadLibrary("egg"); }
    private native int crackEgg(AssetManager assets);

    private AssetManager assets;
    private ClassLoader cl;
    private int dex_cookie;
    private Class innerClass;
    private Method innerMethod = null;

    public Crypt(AssetManager assets, ClassLoader cl)
    {
        this.assets = assets;
        this.cl = cl;
        this.dex_cookie = crackEgg(assets);
    }

    private static Method findMethod(Class clazz, String name)
    {
        Method[] methods = clazz.getDeclaredMethods();
        for (Method m : methods) {
            if (m.getName().equals(name)) {
                return m;
            }
        }
        return null;
    }

    private void prepare()
    {
        // Find DexFile.defineClass
        Method defineClass = null;
        try {
            defineClass = findMethod(Class.forName("dalvik.system.DexFile"),
                    "defineClass");
        } catch (ClassNotFoundException e) {
            return;
        }

        // Allows us to invoke this private method
        defineClass.setAccessible(true);

        // Take out target class
        Object[] args = new Object[4];
        args[0] = "com/example/egg/Egg";
        args[1] = cl;
        args[2] = dex_cookie;
        args[3] = new ArrayList();
        try {
            innerClass = (Class)defineClass.invoke(null, args);
        } catch (Exception e) {
            return;
        }

        innerMethod = findMethod(innerClass, "enc");
    }

    public void encrypt(InputStream in, OutputStream out)
    {
        if (innerMethod == null)
            prepare();
        try {
            innerMethod.invoke(null, in, out);
        } catch (Exception e) {
        }
    }

}
