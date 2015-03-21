$(document).ready(function()
{
	// turn off right click menu
	window.addEventListener("contextmenu", function(e) {
        e.preventDefault();
        return false;
    });

    // variables constructions / initializations
	var name, renderer, cooldown_start_time, loadingbar_width, map_width, map_height;
	var stage = new PIXI.Stage(0xff0000); // has to be in main scope for callbacks to work
	var loadingbar = new PIXI.Graphics(); // see above
	var map = new PIXI.DisplayObjectContainer(); // see above
	var waiting_screen = new PIXI.Text("Waiting for other players",{font: 'bold 36px Georgia', fill: 'white'}); // see above
	var game_width = window.innerWidth;
	var game_height = window.innerHeight;
	var shoot_enabled = false;
	var socket = new WebSocket('ws://130.89.231.61:9000','fris_en_fruitig');
	
	// Write down the game name
	var game_name = new PIXI.Text("Fris en dodelijk",{font: 'bold 36px Georgia', fill: 'white'});
	game_name.anchor.x = game_name.anchor.y = 0.5;
	game_name.position.x = game_width/2;
	game_name.position.y = 5;
	stage.addChild(game_name);


	// set game screen to full size (also to enable mobile)

	$("canvas").width = window.innerWidth;
	$("canvas").height = window.innerHeight;

	// attempt connecting to server with name

	$("#connectForm").submit(function(event)
	{
		event.preventDefault();
		name = $("#name").val();
		send_to_server("connect "+name);
		$("#connectForm").remove();
	});

	socket.onopen = function() {
		console.log("Connection established");
		$("body").css('background-color', '#0f0');
	}

	socket.onerror = function() {
		$("body").css('background-color', '#f00');
	}

	socket.onclose = function() {
		console.log("Connection closed");
		$("body").css('background-color', '#000');
	}

	var message;
	socket.onmessage = function(event) {
		console.log(event);
		message = event.data;
		var index_keyword = message.indexOf(" ");
		var keyword = message.substr(0,index_keyword);
		console.log("Receive: "+message);
		switch (keyword)
		{
			case "ok":
				wait_screen();
				break;

			case "start":
				start_game(message.substr(index_keyword,message.length).split(" "));
				break;

			case "frame":
				draw_frame(message.substr(index_keyword,message.length).split(" "));
				break;
		}
	}

	function wait_screen() {
		waiting_screen.anchor.x = waiting_screen.anchor.y = 0.5;
		waiting_screen.position.x = game_width/2;
		waiting_screen.position.y = game_height/2;
		stage.addChild(waiting_screen);
	}

	function start_game(message) {
		map.removeChild(waiting_screen);
		map_width = message[1];
		map_height = message[2];
		var player_amount = message[3];

		$("body").css('background-color', '#000');
		renderer = PIXI.autoDetectRenderer(game_width, game_height);

		var lava = PIXI.Texture.fromImage("sprites/lava.png");
		var game_background = new PIXI.Sprite(lava);
		game_background.width = game_width;
		game_background.height = game_height;
		stage.addChild(game_background);

		//playing screen
		map.width = map_width;
		map.height = map_height;
		map.position.x = ((game_width/2)-(map_width)/2);
		map.position.y = ((game_height/2)-(map_height)/2);
		stage.addChild(map);

		//playing screen background
		var floor = PIXI.Texture.fromImage("sprites/floor.png");
		var background = new PIXI.Sprite(floor);
		background.width = map_width;
		background.height = map_height;
		map.addChild(background);


		var player_amount_text = new PIXI.Text("Players: "+player_amount, {font: 'normal 12px Georgia', fill: 'white'});
		player_amount_text.anchor.x = player_amount_text.anchor.y = 1;
		player_amount_text.position.x = map_width-5;
		player_amount_text.position.y = map_height-5;
		map.addChild(player_amount_text);

		document.body.appendChild(renderer.view);
		draw();

		shoot_enabled = true;
		setTimeout(enableShoot(),3000);
	}

	function draw_frame(message) {
		message.splice(0,1);
		var entities = JSON.parse(message);

		var player_texture = PIXI.Texture.fromImage("sprites/soldier.png");
		var tree_texture = PIXI.Texture.fromImage("sprites/tree.png");
		var bullet_texture = PIXI.Texture.fromImage("sprites/bullet.png");

		player_counter = 2; // semi-random value to generate nice colours for players
		for (var i = 0; i < entities.length; i++)
		{
			var entity = entities[i];
			switch(entity[0])
			{
				case "player":
					var player = new PIXI.Sprite(player_texture);
					player.tint = 1/player_counter * 0xffffff;
					player.anchor.x = player.anchor.y = 0.5;
					player.position.x = entity.x;
					player.position.y = entity.y;
					player.rotation = entity.angle;
					player.width = player.height = 10;
					map.addChild(player);
					player_counter += 3;
					break;

				case "tree":
					var tree = new PIXI.Sprite(tree_texture);
					tree.anchor.x = tree.anchor.y = 0.5;
					tree.position.x = entity.x;
					tree.position.y = entity.y;
					tree.width = tree.height = 20;
					map.addChild(tree);
					break;

				case "bullet":
					var bullet = new PIXI.Sprite(bullet_texture);
					bullet.anchor.x = bullet.anchor.y = 0.5;
					bullet.position.x = entity.x;
					bullet.position.y = entity.y;
					bullet.position = entity.angle;
					bullet.width = bullet.height = 5;
					map.addChild(bullet);
					break;
			}
		}
		draw();
	}
	
	function draw() {
		var now = Date.now();
		loadingbar.width = ((now-cooldown_start_time)/3000)*loadingbar_width;
		requestAnimFrame(draw);
		renderer.render(stage);
	}

	stage.tap = stage.click = function(event) {
		alert(event.getLocalPosition(map).x+", "+event.getLocalPosition(map).y);
		send_to_server("input l "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
	}

	stage.rightclick = function(event) {
		if (shoot_enabled)
		{
			send_to_server("input r "+event.getLocalPosition(map).x+" "+event.getLocalPosition(map).y);
			shootCooldown();
			setTimeout(enableShoot(),3000);
		}
	}

	function shootCooldown()
	{
		cooldown_start_time = Date.now();
		shoot_enabled = false;
		loadingbar.beginFill(0x1D428A);
		loadingbar.drawRect(0,map_height, 0, 15);
		loadingbar_width = 60;
		alert(loadingbar_width);
		map.addChild(loadingbar);
	}

	function enableShoot()
	{
		shoot_enabled = true;
		map.removeChild(loadingbar);
	}

	function send_to_server(message)
	{
		console.log("Send: "+message);
		socket.send(message);
	}
});