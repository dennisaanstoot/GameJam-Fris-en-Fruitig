function music() {
	var mainTune = "Tune";
	createjs.Sound.registerSound("music/BgmBa4Darkon.mp3", mainTune);
	createjs.Sound.play(mainTune);
}