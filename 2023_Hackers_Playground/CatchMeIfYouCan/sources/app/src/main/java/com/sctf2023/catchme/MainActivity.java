package com.sctf2023.catchme;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.wear.widget.WearableLinearLayoutManager;
import androidx.wear.widget.WearableRecyclerView;


public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView textView = findViewById(R.id.text);

        final WearableRecyclerView rotary = findViewById(R.id.rotary);
        rotary.setHasFixedSize(true);
        rotary.setEdgeItemsCenteringEnabled(true);
        rotary.setLayoutManager(new WearableLinearLayoutManager(this));

        rotary.setAdapter(new RotaryAdapter(new RotaryAdapter.Callback() {
            @Override
            public void onClicked(@NonNull RotaryAdapter.Item item) {
                final String text = textView.getText().toString();
                final String input = text + item.getText();
                textView.setText(input);
                Toast.makeText(MainActivity.this, item.getText(), Toast.LENGTH_LONG).show();
                onUpdated(input);
            }

            private void onUpdated(@NonNull String input) {
                if (new Scrambler().verify(input.getBytes())) {
                    rotary.setVisibility(View.INVISIBLE);
                    Toast.makeText(MainActivity.this, "Bingo!", Toast.LENGTH_LONG).show();
                }
            }
        }));
    }
}