NRev reverb => dac;
reverb => NRev r2 => dac;
0.05 => r2.gain;
0.0 => r2.mix;
0.1 => reverb.mix;
OscSend xmit;
"localhost" => string hostname;
8081 => int port;
xmit.setHost( hostname, port );

OscRecv recv;
8080 => recv.port;
recv.listen();
recv.event( "/addStar, ii" ) @=> OscEvent addStar;
recv.event( "/endConst, i" ) @=> OscEvent endConst;

16 => int granularity;
[60,62,63,65,67,68,70,72,74,75,77,79,80,82,84,86,87] @=> int notes[];
Event pulse;
100=> int pulseTime;

spork ~ globalpulse();

fun void globalpulse(){
    while(true){
        pulseTime::ms => now;
        pulse.broadcast();
        //spork ~ playNote(48);   
    }
}

class Star{
    
    int x, y; 
    int t;
    
    fun void init(int _x, int _y, int _t){
        
        _x => x;
        _y => y;
        //if (_t == 0) 1 => _t;
        _t => t;   
    }  
}

class Constellation{
    
    Star stars[0];
    0 => int counter;
    0 => int lastCount;
    int IDNumber;
    int endCount;
    true => int isEmpty;
    false => int terminatePulse;
    
    false => int Playback;
    
    fun void starPulse(){
        while(!terminatePulse){
            pulse => now;
            counter++;
            lastCount++; 
        }
    }
    
    fun void init(int id){
     id => IDNumber; 
     stars.clear();
     0 => lastCount;
     0 => counter;
     0 => endCount;
     true => isEmpty;
     false => terminatePulse; 
    }
    
    fun void startConstellation(int x, int y){
        spork ~ starPulse();
        <<<"Constellation Started">>>;
        false => isEmpty;
        spork ~ playNote(x - (IDNumber * 12),y);
        oscAddStar(x,y);

        Star s;
        s.init(x,y,lastCount);
            <<<"Star added with time  :  " + lastCount>>>;
            stars << s;
        
    }
    
    fun void endConstellation(){
        
        counter => endCount;
            Star s;
            s.init(stars[0].x,stars[0].y,lastCount);
           // oscAddStar(stars[0].x,stars[0].y);

            lastCount => stars[0].t;
            <<<"First added with time  :  " + lastCount>>>;
           // stars << s;
        true => terminatePulse;
        
        
    }
    
    fun void addStar(int x, int y){
        if (isEmpty){
            startConstellation(x,y);
        }
        else{
            Star s;
            s.init(x,y,lastCount);
            <<<"Star added with time  :  " + lastCount>>>;
            stars << s;
            0 => lastCount;
            oscAddStar(x,y);
            spork ~ playNote(x,y);
        }
    }
    
    fun void removeStar(){
        if (stars.size() > 0) stars.popBack();  
    }  
}

fun void playBackConstellation(Constellation con){
    
    0 => int starIndex;
    0 => int timeGuy;
    0 => int timeGuy2;
    0 => int totalCount;
    true => con.Playback;
    
    while(con.Playback){
        
        con.stars[starIndex].x => float xVal;
        con.stars[starIndex].y => float yVal;
        if ((con.stars[starIndex].t == timeGuy)){
            
            // if(starIndex != con.stars.size()) 
        spork ~ playNote(con.stars[starIndex].x,con.stars[starIndex].y);
        //spork ~ interpolator(con.stars[starIndex],con.stars[tmod(starIndex + 1,con.stars.size())],con.IDNumber);
        //spork ~ interp(con.stars[starIndex],con.stars[tmod(starIndex + 1,con.stars.size())],con.IDNumber);
        spork ~ interp2(starIndex,con);
        (starIndex + 1) % con.stars.cap() => starIndex;
        //if (starIndex == con.stars.size()) 0 => starIndex;
        //timeGuy +=> totalCount;
        0 => timeGuy; 
        if (starIndex == 0) <<<"first Star">>>;
        else <<<"Current Star is :  " + starIndex>>>;
    }
    (timeGuy+1) % con.endCount => timeGuy; 
    //(timeGuy2+1) % con.endCount => timeGuy2;
    if (timeGuy == 0){
        0 => starIndex;
        0 => timeGuy;
    }
    //if (totalCount == con.endCount) 0 => timeGuy;
    pulse => now;
    
}

if (con.Playback){
    me.exit();   
}
}
0 => int cC;
Constellation consts[4];
consts[cC] @=> Constellation c;
c.init(cC);



spork ~ OSCListener1();
spork ~ OSCListener2();


fun void playNote(int x, int y){ 
    Rhodey r => ADSR j => reverb;
    j.set(y * 12::ms, 100::ms,1.0,300::ms);
    0.3 => r.gain;
    Std.mtof(notes[x]) => r.freq;
    0.6 => r.noteOn;
    j.keyOn();
    1300::ms => now;
    j.keyOff();
    0 => r.noteOn;
    1500::ms => now;
}

fun void oscAddStar(int x, int y){
    xmit.startMsg( "/addStar", "ii" );
                x => xmit.addInt;
                y => xmit.addInt;
    
}

fun void interpolator(Star s1, Star s2, int id){
    
    s1.t => int myDuration;
    pulseTime / 20.0 => float fraction;
    //fraction / 10 => fraction;
    //1200::ms => now;
    for (0 => float i; i < pulseTime*myDuration; 10 + i => i)
    {
            xmit.startMsg( "/pPosition", "ffi" );
            CosineInterpolation(s1.x,s2.x,i/myDuration) => float x;
            CosineInterpolation(s1.y,s2.y,i/myDuration) => float y;
            x => xmit.addFloat;
            y => xmit.addFloat;
            id => xmit.addInt;
            (10/pulseTime)::ms => now;   
    }  
}

fun void interp(Star s1, Star s2, int id)
{
 xmit.startMsg("/pPosition", "ffffii");
 s1.x => xmit.addFloat;
 s1.y => xmit.addFloat;
 s2.x => xmit.addFloat;
 s2.y => xmit.addFloat;
 id => xmit.addInt;
 s1.t => xmit.addInt;
 <<<"Sent Message">>>;
}

fun void interp2(int index, Constellation conn)
{
    xmit.startMsg("/pPosition", "iii");
    index => xmit.addInt;
    conn.stars[index].t => xmit.addInt;
    conn.IDNumber => xmit.addInt;
    <<<index>>>;
}


fun float CosineInterpolation(
float y2,float y1,
float mu)
{
    if (mu > 1) 1 => mu;
    float mu2;
    (1-Math.cos(mu*Math.PI))/2 =>  mu2;
    return(y1*(1-mu2)+y2*mu2);
}

fun int tmod(int a, int b) {
    if (a >= b) return a-b;
    if (a < 0) return b+a;
    return a;
}

fun void OSCListener1(){
    while ( true )
    {
        // wait for event to arrive
        addStar => now;
        pulse => now;
        
        // grab the next message from the queue. 
        if ( addStar.nextMsg() != 0 )
        { 
            addStar.getInt() => int x;
            addStar.getInt() => int y;
            c.addStar(x,y);  
        }
    }
} 

fun void OSCListener2(){ 
    while ( true ) 
    {
        endConst => now;
        pulse => now;
        while (endConst.nextMsg() != 0){
            endConst.getInt();  
            c.endConstellation();
            spork ~ playBackConstellation(c);
            startNewConstellation();
        }
    }
}

fun void startNewConstellation(){
 
 (cC + 1) % consts.size() => cC;
 consts[cC] @=> c;
 c.init(cC);
 xmit.startMsg( "/addConstellation", "i" );
            cC => xmit.addInt;
    
}

while(true){
 3::minute => now;   
}



