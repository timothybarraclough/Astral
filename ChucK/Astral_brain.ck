//——————————————————————————————————————————————// AUDIO Setup /
NRev reverb => dac;
reverb => NRev r2 => dac;
0.15 => r2.gain;
0.8 => r2.mix;
0.2 => reverb.mix;

//Note Selection
[60,62,63,65,67,69,70,72,74,75,77,79,81,82,84,86,87] @=> int notes[];


//——————————————————————————————————————————————// OSC Variables /

/*Osc Output port setup*/
    OscSend xmit;
    "localhost" => string hostname;
    8081 => int port;
    xmit.setHost( hostname, port );

/*Osc Output port setup*/
    OscRecv recv;
    8080 => recv.port;
    recv.listen();
    recv.event( "/addStar, ii" ) @=> OscEvent addStar;
    recv.event( "/endConst, i" ) @=> OscEvent endConst;
    recv.event( "/resetEverythingOF, s" ) @=> OscEvent resetEverythingOF;

//——————————————————————————————————————————————// Application Variables /

16 => int granularity;
4 => int constAmount;
Event pulse;
200=> int pulseTime;
0 => int cC;
Constellation consts[4];

//——————————————————————————————————————————————// Initial Handler Sporking /

spork ~ ofReset();
resetEverythingCK();
spork ~ globalpulse();

fun void ofReset(){
    
}

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
    Event heatDeath;
    
    fun void starPulse(){
        while(!terminatePulse){
            pulse => now;
            counter++;
            lastCount++; 
        }
    }
    
    fun void init(int id){
        
        id => IDNumber; 
        0 => lastCount;
        0 => counter;
        0 => endCount;
        true => isEmpty;
        false => terminatePulse; 
        false => Playback;
        while (stars.size() > 0) stars.popBack();
    }
    
    fun void destructor(int id){
        this.destroy();
    }
    
    fun void startConstellation(int x, int y){
        spork ~ starPulse();
        false => isEmpty;
        spork ~ playNote(x,y,this);
        oscAddStar(x,y);
        Star s;
        s.init(x,y,lastCount);
        stars << s;
    }
    
    fun void endConstellation(int theIndex){
        
        counter => endCount;
        Star s;
        s.init(stars[0].x,stars[0].y,lastCount);
        lastCount => stars[0].t;
        true => terminatePulse;
        this @=> consts[theIndex]; 
    }
    
    fun void addStar(int x, int y){
        if (isEmpty){
            startConstellation(x,y);
        }
        else{
            Star s;
            s.init(x,y,lastCount);
            //  <<<"Star added with time  :  " + lastCount>>>;
            stars << s;
            0 => lastCount;
            oscAddStar(x,y);
            spork ~ playNote(x,y,this);
        }
    }
    
    fun void removeStar(){
        if (stars.size() > 0) stars.popBack();  
    }  
    
    fun void destroy(){
        while (stars.size() > 0) stars.popBack();
    }
}

fun void playBackSingleStar(Constellation con){
    true => con.Playback;
    spork ~ deathHandle(me.id(),con);
    while (con.Playback){
        spork ~ playNote(con.stars[0].x,con.stars[0].y,con);
        for (0 => int i; i < con.stars[0].t; i++){
            pulse => now;   
        }
    }
}

fun void playBackConstellation(Constellation con){
    
    1 => int starIndex;
    0 => int timeGuy;
    0 => int timeGuy2;
    0 => int totalCount;
    true => con.Playback;
    spork ~ playNote(con.stars[0].x,con.stars[0].y,con);
    spork ~ interp2(0,con);
    spork ~ deathHandle(me.id(),con);
    
    while(true)
    {
        
        con.stars[starIndex].x => float xVal;
        con.stars[starIndex].y => float yVal;
        if ((con.stars[starIndex].t == timeGuy))
        {
            
            // if(starIndex != con.stars.size()) 
            spork ~ playNote(con.stars[starIndex].x,con.stars[starIndex].y,con);
        //spork ~ interpolator(con.stars[starIndex],con.stars[tmod(starIndex + 1,con.stars.size())],con.IDNumber);
        //spork ~ interp(con.stars[starIndex],con.stars[tmod(starIndex + 1,con.stars.size())],con.IDNumber);
        if (con.Playback ) spork ~ interp2(starIndex,con);
        (starIndex + 1) % con.stars.cap() => starIndex;
        //if (starIndex == con.stars.size()) 0 => starIndex;
        //timeGuy +=> totalCount;
        0 => timeGuy; 
        // if (starIndex == 0) <<<"first Star">>>;
        // else <<<"Current Star is :  " + starIndex>>>;
    }
    (timeGuy+1) % con.endCount => timeGuy; 
    //(timeGuy2+1) % con.endCount => timeGuy2;
    if (timeGuy == 0){
        0 => starIndex;
        0 => timeGuy;
    }
    pulse => now;
    
    
}
}

fun void deathHandle(int id, Constellation con){
    con.heatDeath => now;
    <<<"HeatDeath has occured">>>;
    Machine.remove(id);
    me.exit();
}


Constellation c;
c.init(cC);
xmit.startMsg( "/addConstellation", "i" );
cC => xmit.addInt;

//1 +=> cC;



spork ~ OSCListener1();
spork ~ OSCListener2();


fun void playNote(int x, int y, Constellation con){ 
    Rhodey r => ADSR j => reverb;
    j.set((62 * y)::ms, (80 * y)::ms,1.0,300::ms);
    0.3 => r.gain;
    Std.mtof(notes[x] - (con.IDNumber * 12)) => r.freq;
    0.6 => r.noteOn;
    j.keyOn();
    ((62*y) + 30)::ms => now;
    j.keyOff();
    0 => r.noteOn;
    ((150*y) + 30)::ms => now;
}

fun void oscAddStar(int x, int y){
    xmit.startMsg( "/addStar", "ii" );
    x => xmit.addInt;
    y => xmit.addInt;
    
}

fun void interp2(int index, Constellation conn)
{
    if (!conn.isEmpty){
        xmit.startMsg("/pPosition", "iii");
        index => xmit.addInt;
        conn.stars[index].t => xmit.addInt;
        conn.IDNumber => xmit.addInt;
    }
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
    {   // wait for event to arrive
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
        consts[cC].heatDeath.broadcast();
        if( endConst.nextMsg() != 0){
            endConst.getInt() => int value;
            c.endConstellation(value);
                        c @=> consts[value];
            if (consts[value].stars.size() > 1){
            spork ~ playBackConstellation(consts[value]);
        }
        else spork ~ playBackSingleStar(consts[value]);
            <<<" Old constellation terminated at index  :  " + cC>>>;
            startNewConstellation();
        }
        
    }
}

// This is sent whenever a constellation is finished. this may not be such a good idea though.
fun void startNewConstellation(){
    xmit.startMsg( "/addConstellation", "i" );
    cC => xmit.addInt;  
    (cC + 1) % constAmount => cC;
    <<<"New constellation is going at the index  :  " + cC>>>;
    Constellation j;
    j.init(cC);
    j @=> c;
}

fun void resetEverythingCK(){
    xmit.startMsg( "/resetEverythingCK", "s" );
    "reset" => xmit.addString;  
}

while(true){
    3::minute => now;   
}



