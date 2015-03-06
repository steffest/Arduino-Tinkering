$(document).ready(function() {
	$(".dot").click(function() {
		if ($(this).parent().hasClass('tristate')){
			var targetClass="active"
			if ($(this).hasClass("active")){targetClass="doubleactive"}
			if ($(this).hasClass("doubleactive")){targetClass=""}
			$(this).removeClass("active");
			$(this).removeClass("doubleactive");
			this.className = this.className + " " + targetClass;
		}else{
			$(this).toggleClass("active");
		}
		setpattern();
	});
});

function setpattern(){
	var d;
	var pattern = ""
	// track 1-2 tristate
	for (i=1;i<=16;i++){
		d = getdoublepatternvalue(i,1);
		pattern += d;
	}
	
	// track 3-4 tristate
	for (i=1;i<=16;i++){
		d = getdoublepatternvalue(i,2);
		pattern += d;
	}
	
	// track 5-6
	for (i=1;i<=16;i++){
		d = getpatternvalue(i,2);
		pattern += d;
	}

	$("#pattern1").get(0).value = pattern;
	
}

function getdoublepatternvalue(p,section){
	var sectiontrack = (section-1)*2
	
	var d1 = $("#t" + (sectiontrack+1) + "d" + p).hasClass('active');
	var d2 = $("#t" + (sectiontrack+1) + "d" + p).hasClass('doubleactive');
	var d3 = $("#t" + (sectiontrack+2) + "d" + p).hasClass('active');
	var d4 = $("#t" + (sectiontrack+2) + "d" + p).hasClass('doubleactive');
	
	var d = "-"
	if (d1 && !d2 && !d3 && !d4){d = "1"}
	if (!d1 && d2 && !d3 && !d4){d = "2"}
	if (d1 && d2 && !d3 && !d4) {d = "3"}
	if (!d1 && !d2 && d3 && !d4){d = "4"}
	if (d1 && !d2 && d3 && !d4) {d = "5"}
	if (!d1 && d2 && d3 && !d4) {d = "6"}
	if (d1 && d2 && d3 && !d4)  {d = "7"}
	if (!d1 && !d2 && !d3 && d4){d = "8"}
	if (d1 && !d2 && !d3 && d4) {d = "9"}
	if (!d1 && d2 && !d3 && d4) {d = "a"}
	if (d1 && d2 && !d3 && d4)  {d = "b"}
	if (!d1 && !d2 && d3 && d4) {d = "c"}
	if (d1 && !d2 && d3 && d4)  {d = "d"}
	if (!d1 && d2 && d3 && d4)  {d = "e"}
	if (d1 && d2 && d3 && d4)   {d = "f"}
	
	return d;
}


function getpatternvalue(p,section){
	var sectiontrack = (section-1)*4
	
	var d1 = $("#t" + (sectiontrack+1) + "d" + p).hasClass('active');
	var d2 = $("#t" + (sectiontrack+2) + "d" + p).hasClass('active');
	var d3 = $("#t" + (sectiontrack+3) + "d" + p).hasClass('active');
	var d4 = $("#t" + (sectiontrack+4) + "d" + p).hasClass('active');
	
	var d = "-"
	if (d1 && !d2 && !d3 && !d4){d = "1"}
	if (!d1 && d2 && !d3 && !d4){d = "2"}
	if (d1 && d2 && !d3 && !d4) {d = "3"}
	if (!d1 && !d2 && d3 && !d4){d = "4"}
	if (d1 && !d2 && d3 && !d4) {d = "5"}
	if (!d1 && d2 && d3 && !d4) {d = "6"}
	if (d1 && d2 && d3 && !d4)  {d = "7"}
	if (!d1 && !d2 && !d3 && d4){d = "8"}
	if (d1 && !d2 && !d3 && d4) {d = "9"}
	if (!d1 && d2 && !d3 && d4) {d = "a"}
	if (d1 && d2 && !d3 && d4)  {d = "b"}
	if (!d1 && !d2 && d3 && d4) {d = "c"}
	if (d1 && !d2 && d3 && d4)  {d = "d"}
	if (!d1 && d2 && d3 && d4)  {d = "e"}
	if (d1 && d2 && d3 && d4)   {d = "f"}
	
	return d;
}



function sendpattern(){
	var url = "http://home.stef.be:4998/serial/com10/" + $("#pattern1").get(0).value;
	$("#bridge").get(0).src = url;
}