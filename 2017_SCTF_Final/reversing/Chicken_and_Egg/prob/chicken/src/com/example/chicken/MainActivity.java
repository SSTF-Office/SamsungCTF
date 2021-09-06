package com.example.chicken;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Base64;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

class DecHandler implements OnClickListener
{
    private MainActivity a;
    public DecHandler(MainActivity a)
    {
        this.a = a;
    }

    @Override
    public void onClick(View v)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(a);
        builder.setTitle("DLC");
        String content = a.getResources().getString(R.string.popup_content);
        builder.setMessage(content);

        builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                String url = new String(
                        Base64.decode("aHR0cDovL2tyZGljLm5hdmVyLmNvbS9kZXRhaWwubmhuP2RvY2lkPTY3ODkzMDA=", 0));
                Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
                a.startActivity(browserIntent);
                dialog.dismiss();
            }
        });
        builder.show();
    }
}

public class MainActivity extends Activity implements OnClickListener
{
    private Crypt crypt = null;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Button btnEnc = (Button)findViewById(R.id.btn_enc);
        btnEnc.setOnClickListener(this);
        Button btnDec = (Button)findViewById(R.id.btn_dec);
        btnDec.setOnClickListener(new DecHandler(this));
    }

    public void toast(String s)
    {
        Toast.makeText(getApplicationContext(), s, 10000).show();
    }

    @Override
    public void onClick(View v)
    {
        if (crypt == null)
            crypt = new Crypt(getAssets(), getClassLoader());
        showFileChooser();
    }

    private void showFileChooser()
    {
        ((TextView)findViewById(R.id.main_text)).setText("...");
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("*/*");
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        try {
            startActivityForResult(
                    Intent.createChooser(intent, "Select a file to encrypt"),
                    42);
        } catch(android.content.ActivityNotFoundException ex) {
            toast("Please install a file manager.");
        }
    }

    protected void onActivityResult(int req, int res, Intent data)
    {
        if (req == 42 && res == RESULT_OK)
        {
            Uri uri = data.getData();

            final InputStream in;
            final File outpath;
            final FileOutputStream out;

            try {
                in = getContentResolver().openInputStream(uri);
                outpath = new File(getExternalFilesDir(null), "output.enc");
                out = new FileOutputStream(outpath);
            } catch (FileNotFoundException e) {
                toast("Error opening file");
                return;
            }
            crypt.encrypt(in, out);
            ((TextView)findViewById(R.id.main_text)).setText("written to " + outpath.getPath());
        }
    }

}
