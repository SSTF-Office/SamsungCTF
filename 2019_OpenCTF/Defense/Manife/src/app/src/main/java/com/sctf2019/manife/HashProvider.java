package com.sctf2019.manife;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * @author vkarko
 * @since 30.05.19 18:21
 */
public class HashProvider extends ContentProvider {
    public static final String RESULT = "RESULT";
    private final HashBuilder builder;

    public HashProvider() throws NoSuchAlgorithmException {
        builder = new HashBuilder(MessageDigest.getInstance("SHA-256"));
    }

    @Override
    public boolean onCreate() {
        builder.excludeAttribute("application", "debuggable")
               .excludeAttribute("application", "supportsRtl")
               .excludeAttribute("application", "taskAffinity")
               .excludeAttribute("application", "allowBackup")
               .excludeAttribute("application", "testOnly");
        return true;
    }

    @Nullable
    @Override
    public Bundle call(@NonNull String method, @Nullable String arg, @Nullable Bundle extras) {
        if (!"hash".equals(method)) return null;

        final Context context = getContext();
        if (context == null) return null;

        final HashNode root = builder.build(context);

        final Bundle result = new Bundle();
        result.putString(RESULT, root.value);
        return result;
    }

    //<editor-fold desc="Unsupported">
    @Override
    public int delete(@NonNull Uri uri, String selection, String[] selectionArgs) {
        // Implement this to handle requests to delete one or more rows.
        throw new UnsupportedOperationException();
    }

    @Override
    public String getType(@NonNull Uri uri) {
        throw new UnsupportedOperationException();
    }

    @Override
    public Uri insert(@NonNull Uri uri, ContentValues values) {
        throw new UnsupportedOperationException();
    }

    @Override
    public Cursor query(@NonNull Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        throw new UnsupportedOperationException();
    }

    @Override
    public int update(@NonNull Uri uri, ContentValues values, String selection, String[] selectionArgs) {
        throw new UnsupportedOperationException();
    }
    //</editor-fold>
}
