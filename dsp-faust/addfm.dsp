import("stdfaust.lib");

harmFm(cFreq,hRatio,idx) = os.osc(cFreq + os.osc(cFreq * hRatio) * idx * hRatio);

fm = hgroup("[0]FM", harmFm(carFreq, harmRatio, index))
with{
    harmRatio = nentry("[0]harmonic_ratio",1,0,64,0.01) : si.smoo;
    index = nentry("mod_index", 100, 0, 8800, 0.01) : si.smoo;
    carFreq = nentry("freq", 440, 0, 8800, 0.01) : si.smoo;
};
envelope = hgroup("[1]envelope", en.adsr(attack,decay,sustain,release,gate) * gain * 0.3)
with{
    attack = nentry("[0]attack", 50,1,1000,1) * 0.001 : si.smoo;
    decay = nentry("[1]decay", 50,1,1000,1) * 0.001 : si.smoo;
    sustain = nentry("[2]sustain", 0.8,0.01,1,0.01) : si.smoo;
    release = nentry("[3]release", 750,1,1000,1) * 0.001;
    gain = nentry("[4]gain",1,0,1,0.01) : si.smoo;
    gate = button("[5]gate") : si.smoo;
};

process = fm * envelope <: _;