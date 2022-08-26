objects = {
    apples = 0;
}

merchant = {
    id = "merchant",
    text = "You have no apples.",
    onEnter = function(self)
        if objects.apples <= 0 then
            self.text = "You have no apples."
        elseif objects.apples == 1 then
            self.text = "You have an apple"
        elseif objects.apples == 5 then
            self.text = "Dude.. Why do you need 5 apples?.."
        else
            self.text = "You have the " .. objects.apples .. " apples."
        end
    end,
    choices = {
        {
            id = "buy",
            text = "Buy an apple",
            isEnabled = function(self, super)
                return objects.apples < 5
            end,
            onSelect = function(self, super)
                objects.apples = objects.apples + 1
            end,
        }, {
            id = "sell",
            text = "Sell an apple",
            isEnabled = function(self, super)
                return objects.apples > 0
            end,
            onSelect = function(self, super)
                objects.apples = objects.apples - 1
            end,
        },
    }
}

start(merchant)
