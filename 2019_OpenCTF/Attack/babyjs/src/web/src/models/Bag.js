
class Bag {
    constructor() {
    }

    SuperPower(describe) {
      if(describe.length > 30)
        return "No.. Hack";

      try {
        if(eval(describe))
          return "Sauron!!!";
        else 
          return "You are not qualified..";
      } catch(e) {
        return e;
      }
    }

    add(name, describe) {
        if(name == "@neRing")
          this[name] = this.SuperPower(describe);
        else
          this[name] = describe;
    }
  }

  module.exports = Bag
