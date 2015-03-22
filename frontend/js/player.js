/**
 * Created by Langstra on 3/22/2015.
 */
function draw_health(health) {
    var graphics = new PIXI.Graphics();

    graphics.lineStyle(5, 0x000000, 1);
    graphics.drawRect(30, game_height - 600, 110, 30);

    graphics.beginFill(0xff0000, 1);
    graphics.drawRect(35, game_height-605, 100, 20);
    graphics.endFill();

    return graphics;
}