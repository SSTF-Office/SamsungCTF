package com.sctf2023.catchme;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;
import java.util.List;

public class RotaryAdapter extends RecyclerView.Adapter<RotaryAdapter.RecyclerViewHolder> {
    public static class RecyclerViewHolder extends RecyclerView.ViewHolder {
        private final View layout;
        private final TextView textView;

        public RecyclerViewHolder(@NonNull View view) {
            super(view);

            layout = view.findViewById(R.id.rotary_item);
            textView = view.findViewById(R.id.rotary_item_text);
        }
    }

    public static class Item {
        private final String text;

        public Item(@NonNull String text) {
            this.text = text;
        }

        @NonNull
        public String getText() {
            return text;
        }
    }

    public interface Callback {
        void onClicked(@NonNull Item item);
    }

    private final List<Item> data = new ArrayList<>();
    private final Callback callback;


    public RotaryAdapter(@Nullable Callback callback, @NonNull String... texts) {
        this.callback = callback;
        for (String text : texts) {
            data.add(new Item(text));
        }
    }

    public RotaryAdapter(@Nullable Callback callback) {
        this(callback, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
                "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4",
                "5", "6", "7", "8", "9", "{", "_", "}"
        );
    }

    @NonNull
    @Override
    public RecyclerViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.rotary_item, parent, false);
        return new RecyclerViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerViewHolder holder, int position) {
        final Item item = data.get(position);

        holder.textView.setText(item.getText());
        holder.layout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (callback != null) {
                    callback.onClicked(item);
                }
            }
        });
    }

    @Override
    public int getItemCount() {
        return data.size();
    }
}
