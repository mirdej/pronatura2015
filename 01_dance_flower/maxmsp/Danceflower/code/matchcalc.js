var e = new Array(4)
var idx = 0;
outlets = 3;
var points;

function expect(a,b,c,d) {
	e[0]= a;
	e[1]= b;
	e[2]= c;
	e[3]= d;
	idx = 0;
	points = 0;
}


function msg_int (n) {
 if (n == e[idx]) {
 	idx++;
	if (points < idx) points = idx;
	outlet(0,"target",idx);
	outlet(0,"petals",1);
	
	if (idx == 4) {
		outlet(1,"bang");
		idx = 0;
	} 
  } else {
	if (idx) {
		if (n != e[idx-1]) {
			for (var n = 1; n < idx+1; n++) {
				outlet(0,"target",n);
				outlet(0,"petals",2);
			}
			idx = 0;
		}
	}
	}
}


function getscore() {
	messnamed('addScore',points);
	outlet(2,points);
}