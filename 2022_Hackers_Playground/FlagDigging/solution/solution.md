# FlagDigging solution

There are several method to solve the challenge.

## Hooking rendering process with renderdoc
https://www.youtube.com/watch?v=T7MPxvX5alA

## Setting translucent shader
Inject hooking script and call some function inside view port function
https://stackoverflow.com/questions/18439897/webgl-fragment-shader-opacity

Before load `js.o.js` javascript, apply js snippet following.

```hook.js
(function() {

    var originalGetContext = HTMLCanvasElement.prototype.getContext;
    
    HTMLCanvasElement.prototype.getContext = function () {
        console.log(`Hooked getContext(${JSON.stringify(arguments)})`);
        var context = originalGetContext.apply(this, arguments);
    
        var originalBindBuffer = context.__proto__.bufferData;
    
        context.__proto__.bufferData = function() {
            
            console.log(`Hooked bufferData inspection`);
            // You can extract coord data and regenerate the asset wire-frame.
            // console.log(`Asset coord data: \n[${arguments[1]}]`);
            return originalBindBuffer.apply(this, arguments);
        }

        var originalViewport = context.__proto__.viewport;
        
        context.__proto__.viewport = function() {
            //Set translucent shader with hooking script
            console.log(`Hooked viewport inspection`);
            console.log(context.SRC_ALPHA);
            context.blendFunc(context.SRC_ALPHA, context.ONE);
            context.enable(context.BLEND);
            context.disable(context.DEPTH_TEST);

            return originalViewport.apply(this, arguments);
        }
    
        return context;
    }
})();
```

## Reverse engineering javascript and extract model data
Inject hooking script and extract second argument of `bufferData` function and extract model data.