/**
 * Created by Langstra on 3/21/2015.
 */

var loader = new PIXI.AssetLoader([
        "sprites/bullet.png",
        "sprites/floor.png",
        "sprites/lava.png",
        "sprites/lava_1.png",
        "sprites/lava_2.png",
        "sprites/pillar.png",
        "sprites/soldier.png",
        "sprites/tree.png",
        "sprites/tree_brown_1.png",
        "sprites/tree_green_1.png",
        "sprites/tree_orange_1.png",
        "sprites/waiting.png"
    ], true
);

loader.on('onProgress', function(e) {
    //returns the percent complete from [0 - 1]
    //which you can multiply by 100 to display as [0 - 100]
    var loader_percentage = (e.content.loaded / e.content.total)*100;

    console.log(loader_percentage);
    $('.loading-assets .progress-bar').css("width", loader_percentage+'%');
    $('.loading-assets .progress-bar').attr("aria-valuenow", loader_percentage);
    $('#loading-percentage').html(loader_percentage);
});

loader.on('onComplete', function(e) {
    head.load('libs/colpick.js', 'css/colpick.css');
    head.ready('libs/colpick.js', 'css/colpick.css');
    setTimeout(
    function()
    {
        $('.loading-assets').hide();
        $('#login-form').show();
        $('#picker').colpick({
            flat:true,
            layout:'hex',
            colorScheme:'dark',
            submit:0,
            onChange:function(hsb,hex,rgb,el,bySetColor) {
                player_color = hex;
                // Fill the text box just if the color was set using the picker, and not the colpickSetColor function.
                if(!bySetColor) $(el).val(hex);
            }
        });
    }, 500);
});

loader.load();