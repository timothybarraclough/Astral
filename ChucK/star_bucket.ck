OscRecv recv;
8080 => recv.port;

OscSend xmit;
"localhost" => string hostname;
8081 => int port;
xmit.setHost( hostname, port );

recv.listen();
recv.event( "/addStar, ii" ) @=> OscEvent addStar;
recv.event( "/endConst, i" ) @=> OscEvent endConst;


int count;
Event pulse;
Constellation c;


spork ~ counter();
spork ~ OSCListener1();
spork ~ OSCListener2();

for(0 => int i; i < c.stars.cap(); i++){
    <<<"Star at  " + i>>>;
    <<<"x coordinate =  " + c.stars[i].t>>>;
}

//c.stars.popBack();

for(0 => int i; i < c.stars.cap(); i++){
    <<<"Star at  " + i>>>;
    <<<"x coordinate =  " + c.stars[i].t>>>;
}

while(true){
 1::minute => now;   
}

fun void counter(){
    while (true){
        
        150::ms => now;
        count++;
        if (c.isCurrent)c.constellationLength++;
        pulse.broadcast();
    }
}

private class Star
{
    0 => int x;
    0 => int y;
    0 => int t;
    
    fun void init(int _x, int _y, int _t)
    {
        _x => x;
        _y => y;
        _t => t;   
    }
}


private class Constellation
{
    Star stars[0];
    false => int constellationAllocated;
    false => int isCurrent;
    0 => int constellationLength;
    0 => int fullLength;
    time startTime;
    0 => int num;
    false => int pb;
    //Method to add a star to the constellation
    fun void addStar(int x, int y)
    {
        
        Star s;
        s.init(x,y,constellationLength);
        <<<" Added Star at  :  " + x + "   /   " + y>>>;
        <<<"Time since constellation started   :"  + constellationLength>>>;
        if (!constellationAllocated) startConstellation();
        stars << s;
        
    }
    //Called when a new constellation has it's first star
    fun void startConstellation()
    { 
        <<<"Constellation started">>>;
        
        true => constellationAllocated;
        true => isCurrent;
        now => startTime;  
    }
    
    fun void endConstellation(int totalTime)
    {
        addStar(stars[0].x,stars[0].y);
        false => isCurrent;
        <<<totalTime>>>;
        true => pb;
        spork ~ playback(constellationLength);
    }
    
    fun void playback(int duration){
        0 => int PlaybackCounter;
        0 => int index;
        Rhodey s => Gain g => dac;
        
        0.3 => s.gain;
        
        while(pb){
            
            
            // stars[index].y / 64.0 => s.gain;
            (PlaybackCounter + 1) % duration => PlaybackCounter;     
            if (stars[index].t == PlaybackCounter)
            {
                Std.mtof(stars[index].x + 60) => s.freq;
                0 => s.noteOn;
                tmod((index + 1),stars.cap()-1) => index;
                1.0 => s.noteOn;
                <<<"change to index  :  " + index>>>;  
            }
            until(pulse){
            stars[index].x => int xcurrent;
            stars[index].y => int ycurrent;
            stars[tmod(index+1,stars.cap())].x => int targetx;
            stars[tmod(index+1,stars.cap())].y => int targety;
            
            PlaybackCounter - stars[index].t => float numerator;
            stars[tmod(index+1,stars.cap())].t  - stars[index].t => float denominator;
            numerator/denominator => float mu;
            cosineInterpolation(xcurrent,targetx,mu) => float halfx;
            cosineInterpolation(ycurrent,targety,mu) => float halfy;
            <<<halfx>>>;
            playBackPositionOut(halfx,halfy);
            5::ms => now;
        }
            
            pulse => now;   
        }
        if (!pb) me.exit();
    }
    
    fun void interp(){
        
    }
    
    fun void removeLastStar(){
        stars.popBack();   
    }
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
        while ( addStar.nextMsg() != 0 )
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
        c.endConstellation(count);
    }
    }
}

fun void playBackPositionOut(float halfx,float halfy){
    
                xmit.startMsg( "/PBP", "ff" );
                halfx => xmit.addFloat;
                halfy => xmit.addFloat;
}

fun float cosineInterpolation(
float y2,float y1,
float mu)
{
    float mu2;
    
    (1-Math.cos(mu*Math.PI))/2 => mu2;
    
    return(y1*(1-mu2)+y2*mu2);
}



