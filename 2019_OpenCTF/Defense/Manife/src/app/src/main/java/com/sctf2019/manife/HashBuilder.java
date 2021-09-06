package com.sctf2019.manife;

import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.XmlResourceParser;
import android.util.Log;

import androidx.annotation.NonNull;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;
import java.nio.charset.Charset;
import java.security.MessageDigest;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static org.xmlpull.v1.XmlPullParser.END_TAG;
import static org.xmlpull.v1.XmlPullParser.START_DOCUMENT;
import static org.xmlpull.v1.XmlPullParser.START_TAG;

/**
 * @author vkarko
 * @since 30.05.19 18:21
 */
public class HashBuilder extends Visitor<HashNode> {
    private final MessageDigest md;
    private final Map<String, Set<String>> excludes = new HashMap<>();

    public HashBuilder(@NonNull MessageDigest md) {
        this.md = md;
    }

    public HashBuilder excludeTag(String name) {
        excludes.computeIfAbsent(name, __ -> new HashSet<>());
        return this;
    }

    public HashBuilder excludeAttribute(String tagName, String attrName) {
        excludes.computeIfAbsent(tagName, __ -> new HashSet<>()).add(attrName);
        return this;
    }

    @Override
    protected boolean onVisit(@NonNull HashNode node) {
        node.value = CryptoUtils.toHex(md.digest(node.value.getBytes(Charset.forName("UTF-8"))));

        if (!node.isEmpty()) {
            byte[] bytes = new byte[md.getDigestLength()];
            for (Node<String> kid : node) {
                bytes = CryptoUtils.xor(bytes, CryptoUtils.toBytes(kid.value));
            }

            node.value = CryptoUtils.toHex(md.digest(CryptoUtils.add(CryptoUtils.toBytes(node.value), bytes)));
        }

        return true;
    }

    @NonNull
    public HashNode build(@NonNull Context context) {
        try {
            final AssetManager assetManager = context.createPackageContext(context.getPackageName(), 0).getAssets();
            final HashNode root = new HashNode();

            try (final XmlResourceParser parser = assetManager.openXmlResourceParser(0, "AndroidManifest.xml")) {
                HashNode current = root;

                for (int event = parser.getEventType(); event != XmlPullParser.END_DOCUMENT; event = parser.nextToken()) {
                    switch (event) {
                        case START_DOCUMENT: break;

                        case START_TAG: {
                            final String tag = parser.getName();
                            final Set<String> banned = excludes.getOrDefault(tag, Collections.emptySet());

                            //noinspection ConstantConditions
                            if (!excludes.containsKey(tag) || !banned.isEmpty()) {
                                current = new HashNode(tag, current);
                                Log.i("HashBuilder", current.toString());

                                for (int i = 0; i < parser.getAttributeCount(); i++) {
                                    final String attribute = parser.getAttributeName(i);

                                    //noinspection ConstantConditions
                                    if (!banned.contains(attribute)) {
                                        final HashNode n = new HashNode(attribute + "=" + parser.getAttributeValue(i), current);
                                        Log.i("HashBuilder", n.toString());
                                    }
                                }
                            }
                        } break;

                        case END_TAG: {
                            if (current != null) {
                                current = current.getParent();
                            }
                        } break;

                        default: throw new UnsupportedOperationException();
                    }
                }
            } catch (IOException | XmlPullParserException e) {
                throw new RuntimeException(e);
            }

            accept(root);
            return root;
        } catch (PackageManager.NameNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
}
