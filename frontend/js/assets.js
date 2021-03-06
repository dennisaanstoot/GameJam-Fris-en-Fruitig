/**
 * Created by Langstra on 3/21/2015.
 */


var pixi_assets = [
    "sprites/bullet.png",
    "sprites/fireball.png",
    "sprites/floor.png",
    "sprites/floor_new.png",
    "sprites/lava.png",
    "sprites/lava_1.png",
    "sprites/lava_2.png",
    "sprites/lava_new.png",
    "sprites/pillar.png",
    "sprites/soldier.png",
    "sprites/tree.png",
    "sprites/tree_brown_1.png",
    "sprites/tree_green_1.png",
    "sprites/tree_orange_1.png",
    "sprites/waiting.png",
    "sprites/hotdog.png",
    //"sprites/explosion.json"
    "sprites/health_pill.png",
    "sprites/right.png",
    "sprites/down.png",
    "sprites/left.png",
    "sprites/top.png",
  ];

var sounds = [
    //{src: 'music/BgmBa4Darkon.mp3', name: 'darkon'},
    //{src: 'music/BgmTh6Flying.mp3', name: 'flying'},
    //{src: 'music/bullet_default.mp3', name: 'bullet_default'}
];


var total_loaded = 0;

createjs.Sound.alternateExtensions = ["mp3"];
createjs.Sound.on("fileload", calculate_loading_percentage);


for(var i in sounds) {
    createjs.Sound.registerSound(sounds[i].src, sounds[i].name);

}

var loader = new PIXI.AssetLoader(pixi_assets, true);

loader.on('onProgress', calculate_loading_percentage);

loader.load();

function calculate_loading_percentage() {
    var loader_percentage = ++total_loaded / (pixi_assets.length + sounds.length) * 100;
    loader_percentage = Math.round(loader_percentage);

    $('.loading-assets .progress-bar').css("width", loader_percentage+'%');
    $('.loading-assets .progress-bar').attr("aria-valuenow", loader_percentage);
    $('#loading-percentage').html(loader_percentage);

    if(loader_percentage == 100) {
        music('darkon')
        head.load('libs/colpick.js', 'css/colpick.css');
        head.ready('libs/colpick.js', 'css/colpick.css');
        setTimeout(show_login_form, 500);
    }
}

function show_login_form()
{
    $('.loading-assets').hide();
    $('#login-form').show();
    $('#picker').colpick({
        flat:true,
        layout:'hex',
        colorScheme:'dark',
        submit:0,
        onChange: function(hsb,hex,rgb,el,bySetColor) {
            $('#color').val(hex);
            // Fill the text box just if the color was set using the picker, and not the colpickSetColor function.
            if(!bySetColor) $('#color').val(hex);
        }
    });
}
