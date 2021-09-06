// Magnific Popup
var MagnificPopup = function() {
    "use strict";

    // Handle Magnific Popup
    var handleMagnificPopup = function() {
        // Image popup - vertical fit
        $(document).ready(function() {
            $('.js_image-popup-vertical-fit').magnificPopup({
                type: 'image',
                closeOnContentClick: true,
                mainClass: 'mfp-img-mobile',
                image: {
                    verticalFit: true,
                }
            });
        });

        // Popup gallery
        $(document).ready(function() {
            $('.js_popup-gallery').magnificPopup({
                delegate: 'a.js_popup-gallery-child',
                type: 'image',
                mainClass: 'mfp-img-mobile',
                gallery: {
                    enabled: true,
                    navigateByImgClick: true,
                    preload: [0, 1] // Will preload 0 - before current, and 1 after the current image
                },
                image: {
                    tError: '<a href="%url%">The image #%curr%</a> could not be loaded.'
                }
            });
        });

        // Multiple Galleries with a single popup
        $(document).ready(function() {
            $('.js_popup-multiple-image').magnificPopup({
                type: 'image',
                tLoading: 'Loading image #%curr%...',
                fixedContentPos: true,
                gallery: {
                    enabled: true,
                    navigateByImgClick: true,
                    arrowMarkup: '<button title="%title%" type="button" class="mfp-arrow mfp-arrow-%dir%"></button>',
                    preload: [0,1], // Will preload 0 - before current, and 1 after the current image
                }
            });
        });

        // Video iframes
        $(document).ready(function() {
            $('.js_popup-youtube, .js_popup-vimeo').magnificPopup({
                disableOn: 700,
                type: 'iframe',
                mainClass: 'mfp-fade',
                removalDelay: 160,
                preloader: false,
                fixedContentPos: true
            });
        });
    }

    return {
        init: function() {
            handleMagnificPopup(); // initial setup for search box
        }
    }
}();

$(document).ready(function() {
    MagnificPopup.init();
});
