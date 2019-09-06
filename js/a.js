function GetId(n) {
	if (n < 0) n += amount;
	if (n >= amount) n -= amount;
	return n;
}
function RedrawTable() {
    var codeBlock = '<table align="center" class="main">'+
	'<tr class="image"><td><button class="side" onclick="GoLeft()"><img id="left" src="img/left.png"></button></td><td class="image_side"><img src="img/'+GetId(cur-1)+'.png"></td><td class="image"><img src="img/'+GetId(cur)+'.png"></td><td class="image_side"><img src="img/'+GetId(cur+1)+'.png"></td><td><button class="side" onclick="GoRight()"><img id="right" src="img/right.png"></button></td></tr>'+
	'<tr class="name"><td></td><td class="side">'+n[GetId(cur-1)]+'</td><td>'+n[GetId(cur)]+'</td><td class="side">'+n[GetId(cur+1)]+'</td></tr>'+
	'<tr class="desc"><td></td><td class="side_text">'+q[GetId(cur-1)]+'</td><td class="mid_text">'+q[GetId(cur)]+'</td><td class="side_text">'+q[GetId(cur+1)]+'</td></tr></table>';
	document.getElementById("chars").innerHTML = codeBlock;
}
function submit_answer() {
	var data = document.getElementsByName("ans")[0].value;
	if (stage == 0 && data != "") {
		stage = 1;
		username = data;
		RedrawAsk();
	}
	else if (stage == 1 && data != "") {
		stage = 2;
		fav = data;
		RedrawAsk();
	}
}
function RedrawAsk() {
	var askBlock;
	switch(stage)
	{
		case 0:
		askBlock = '<p class="question">Привет, как тебя зовут?</p><form><input type="text" name="ans">&nbsp;<input type="button" value="Готово" onclick="submit_answer();"></form>';
		break;
		case 1:
		askBlock = '<p class="question">Хорошо, а как называется твое любимое аниме?</p><form><input type="text" name="ans">&nbsp;<input type="button" value="Готово" onclick="submit_answer();"></form>';
		break;
		case 2:
		askBlock = '<p class="question">Тебя зовут '+username+' и тебе нравится '+fav+', не так ли?</p><button class="last" onclick="location.href=\'goblin.htm\';">Все правильно!</button>';
		break;
	}
	document.getElementById("ask").innerHTML = askBlock;
}
function GoLeft() {
	cur = GetId(cur-1);
	RedrawTable(cur);
}
function GoRight() {
	cur = GetId(cur+1);
	RedrawTable(cur);
}