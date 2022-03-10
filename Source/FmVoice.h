/* ------------------------------------------------------------
name: "addfm"
Code generated with Faust 2.38.8 (https://faust.grame.fr)
Compilation options: -a faustMinimal.h -lang cpp -es 1 -vec -lv 0 -vs 32 -single -ftz 0 -mcd 16
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>
#include <stdio.h>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
    
    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string replaceCharList(std::string str, const std::vector<char>& ch1, char ch2)
        {
            std::vector<char>::const_iterator beg = ch1.begin();
            std::vector<char>::const_iterator end = ch1.end();
            for (size_t i = 0; i < str.length(); ++i) {
                if (std::find(beg, end, str[i]) != end) {
                    str[i] = ch2;
                }
            }
            return str;
        }
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::vector<char> rep = {' ', '#', '*', ',', '/', '?', '[', ']', '{', '}', '(', ')'};
            replaceCharList(res, rep, '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of complete hierarchical 'paths' and zones for each UI item.
 *
 * Simple 'labels' and complete 'paths' (to fully discriminate between possible same
 * 'labels' at different location in the UI hierachy) can be used to access a given parameter.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // setParamValue/getParamValue
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            } else {
                fprintf(stderr, "ERROR : setParamValue '%s' not found\n", path.c_str());
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                fprintf(stderr, "ERROR : getParamValue '%s' not found\n", path.c_str());
                return 0;
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamAddress1(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            for (const auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }
    
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            if (fPathZoneMap.find(str) != fPathZoneMap.end()) {
                return fPathZoneMap[str];
            } else if (fLabelZoneMap.find(str) != fLabelZoneMap.end()) {
                return fLabelZoneMap[str];
            }
            return nullptr;
        }
    
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class ScopedNoDenormals
{
    private:
    
        intptr_t fpsr;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
           asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            _mm_setcsr(static_cast<uint32_t>(fpsr_aux));
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined ( __SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #else
            #if defined(__SSE__)
            #if defined(__SSE2__)
                intptr_t mask = 0x8040;
            #else
                intptr_t mask = 0x8000;
            #endif
            #else
                intptr_t mask = 0x0000;
            #endif
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals();

#endif

/************************** END dsp.h **************************/

// BEGIN-FAUSTDSP


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

class mydspSIG0 {
	
  private:
	
	int iVec0[2];
	int iRec6[2];
	
  public:
	
	int getNumInputsmydspSIG0() {
		return 0;
	}
	int getNumOutputsmydspSIG0() {
		return 1;
	}
	
	void instanceInitmydspSIG0(int sample_rate) {
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			iVec0[l5] = 0;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			iRec6[l6] = 0;
		}
	}
	
	void fillmydspSIG0(int count, float* table) {
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec0[0] = 1;
			iRec6[0] = ((iVec0[1] + iRec6[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec6[0])));
			iVec0[1] = iVec0[0];
			iRec6[1] = iRec6[0];
		}
	}

};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydsp : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fEntry0;
	float fConst2;
	float fRec0_perm[4];
	FAUSTFLOAT fEntry1;
	float fRec2_perm[4];
	FAUSTFLOAT fEntry2;
	float fRec3_perm[4];
	FAUSTFLOAT fEntry3;
	float fRec4_perm[4];
	float fConst3;
	float fRec5_perm[4];
	float fRec1_perm[4];
	FAUSTFLOAT fButton0;
	float fRec8_perm[4];
	float fRec7_perm[4];
	float fConst4;
	FAUSTFLOAT fEntry4;
	float fRec9_perm[4];
	FAUSTFLOAT fEntry5;
	float fRec10_perm[4];
	FAUSTFLOAT fEntry6;
	float fRec11_perm[4];
	int iRec12_perm[4];
	float fConst5;
	FAUSTFLOAT fEntry7;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.3");
		m->declare("compile_options", "-a faustMinimal.h -lang cpp -es 1 -vec -lv 0 -vs 32 -single -ftz 0 -mcd 16");
		m->declare("envelopes.lib/adsr:author", "Yann Orlarey and Andrey Bundin");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "0.1");
		m->declare("filename", "addfm.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.5");
		m->declare("name", "addfm");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "0.3");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.2");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.1");
	}

	virtual int getNumInputs() {
		return 0;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
		mydspSIG0* sig0 = newmydspSIG0();
		sig0->instanceInitmydspSIG0(sample_rate);
		sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
		deletemydspSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (44.0999985f / fConst0);
		fConst2 = (1.0f - fConst1);
		fConst3 = (1.0f / fConst0);
		fConst4 = (0.0441000015f / fConst0);
		fConst5 = (0.00100000005f * fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fEntry0 = FAUSTFLOAT(1.0f);
		fEntry1 = FAUSTFLOAT(440.0f);
		fEntry2 = FAUSTFLOAT(1.0f);
		fEntry3 = FAUSTFLOAT(100.0f);
		fButton0 = FAUSTFLOAT(0.0f);
		fEntry4 = FAUSTFLOAT(50.0f);
		fEntry5 = FAUSTFLOAT(0.80000000000000004f);
		fEntry6 = FAUSTFLOAT(50.0f);
		fEntry7 = FAUSTFLOAT(750.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 4); l0 = (l0 + 1)) {
			fRec0_perm[l0] = 0.0f;
		}
		for (int l1 = 0; (l1 < 4); l1 = (l1 + 1)) {
			fRec2_perm[l1] = 0.0f;
		}
		for (int l2 = 0; (l2 < 4); l2 = (l2 + 1)) {
			fRec3_perm[l2] = 0.0f;
		}
		for (int l3 = 0; (l3 < 4); l3 = (l3 + 1)) {
			fRec4_perm[l3] = 0.0f;
		}
		for (int l4 = 0; (l4 < 4); l4 = (l4 + 1)) {
			fRec5_perm[l4] = 0.0f;
		}
		for (int l7 = 0; (l7 < 4); l7 = (l7 + 1)) {
			fRec1_perm[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 4); l8 = (l8 + 1)) {
			fRec8_perm[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 4); l9 = (l9 + 1)) {
			fRec7_perm[l9] = 0.0f;
		}
		for (int l10 = 0; (l10 < 4); l10 = (l10 + 1)) {
			fRec9_perm[l10] = 0.0f;
		}
		for (int l11 = 0; (l11 < 4); l11 = (l11 + 1)) {
			fRec10_perm[l11] = 0.0f;
		}
		for (int l12 = 0; (l12 < 4); l12 = (l12 + 1)) {
			fRec11_perm[l12] = 0.0f;
		}
		for (int l13 = 0; (l13 < 4); l13 = (l13 + 1)) {
			iRec12_perm[l13] = 0;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("addfm");
		ui_interface->declare(0, "0", "");
		ui_interface->openHorizontalBox("FM");
		ui_interface->declare(&fEntry2, "0", "");
		ui_interface->addNumEntry("harmonic_ratio", &fEntry2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(64.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addNumEntry("freq", &fEntry1, FAUSTFLOAT(440.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8800.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addNumEntry("mod_index", &fEntry3, FAUSTFLOAT(100.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(8800.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
		ui_interface->declare(0, "1", "");
		ui_interface->openHorizontalBox("envelope");
		ui_interface->declare(&fEntry4, "0", "");
		ui_interface->addNumEntry("attack", &fEntry4, FAUSTFLOAT(50.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry6, "1", "");
		ui_interface->addNumEntry("decay", &fEntry6, FAUSTFLOAT(50.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry5, "2", "");
		ui_interface->addNumEntry("sustain", &fEntry5, FAUSTFLOAT(0.800000012f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fEntry7, "3", "");
		ui_interface->addNumEntry("release", &fEntry7, FAUSTFLOAT(750.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fEntry0, "4", "");
		ui_interface->addNumEntry("gain", &fEntry0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fButton0, "5", "");
		ui_interface->addButton("gate", &fButton0);
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* output0_ptr = outputs[0];
		float fSlow0 = (fConst1 * float(fEntry0));
		float fRec0_tmp[36];
		float* fRec0 = &fRec0_tmp[4];
		float fSlow1 = (fConst1 * float(fEntry1));
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fSlow2 = (fConst1 * float(fEntry2));
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		float fSlow3 = (fConst1 * float(fEntry3));
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fZec0[32];
		float fRec5_tmp[36];
		float* fRec5 = &fRec5_tmp[4];
		float fZec1[32];
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fSlow4 = (fConst1 * float(fButton0));
		float fRec8_tmp[36];
		float* fRec8 = &fRec8_tmp[4];
		float fRec7_tmp[36];
		float* fRec7 = &fRec7_tmp[4];
		float fSlow5 = (fConst4 * float(fEntry4));
		float fRec9_tmp[36];
		float* fRec9 = &fRec9_tmp[4];
		float fSlow6 = (fConst1 * float(fEntry5));
		float fRec10_tmp[36];
		float* fRec10 = &fRec10_tmp[4];
		float fSlow7 = (fConst4 * float(fEntry6));
		float fRec11_tmp[36];
		float* fRec11 = &fRec11_tmp[4];
		int iRec12_tmp[36];
		int* iRec12 = &iRec12_tmp[4];
		float fZec2[32];
		float fSlow8 = (1.0f / std::max<float>(1.0f, (fConst5 * float(fEntry7))));
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; (vindex <= (count - 32)); vindex = (vindex + 32)) {
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			int vsize = 32;
			/* Recursive loop 0 */
			/* Pre code */
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec2_tmp[j2] = fRec2_perm[j2];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec2[i] = (fSlow1 + (fConst2 * fRec2[(i - 1)]));
			}
			/* Post code */
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec2_perm[j3] = fRec2_tmp[(vsize + j3)];
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec3_tmp[j4] = fRec3_perm[j4];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec3[i] = (fSlow2 + (fConst2 * fRec3[(i - 1)]));
			}
			/* Post code */
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec3_perm[j5] = fRec3_tmp[(vsize + j5)];
			}
			/* Recursive loop 2 */
			/* Pre code */
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec4[i] = (fSlow3 + (fConst2 * fRec4[(i - 1)]));
			}
			/* Post code */
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				fRec4_perm[j7] = fRec4_tmp[(vsize + j7)];
			}
			/* Recursive loop 3 */
			/* Pre code */
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec5_tmp[j8] = fRec5_perm[j8];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec0[i] = (fRec5[(i - 1)] + (fConst3 * (fRec2[i] * fRec3[i])));
				fRec5[i] = (fZec0[i] - std::floor(fZec0[i]));
			}
			/* Post code */
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec5_perm[j9] = fRec5_tmp[(vsize + j9)];
			}
			/* Recursive loop 4 */
			/* Pre code */
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				fRec8_tmp[j12] = fRec8_perm[j12];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec8[i] = (fSlow4 + (fConst2 * fRec8[(i - 1)]));
			}
			/* Post code */
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				fRec8_perm[j13] = fRec8_tmp[(vsize + j13)];
			}
			/* Recursive loop 5 */
			/* Pre code */
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fRec9_tmp[j16] = fRec9_perm[j16];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec9[i] = (fSlow5 + (fConst2 * fRec9[(i - 1)]));
			}
			/* Post code */
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fRec9_perm[j17] = fRec9_tmp[(vsize + j17)];
			}
			/* Recursive loop 6 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec0_tmp[j0] = fRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec0[i] = (fSlow0 + (fConst2 * fRec0[(i - 1)]));
			}
			/* Post code */
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec0_perm[j1] = fRec0_tmp[(vsize + j1)];
			}
			/* Recursive loop 7 */
			/* Pre code */
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec1_tmp[j10] = fRec1_perm[j10];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec1[i] = (fRec1[(i - 1)] + (fConst3 * (fRec2[i] + ((fRec3[i] * fRec4[i]) * ftbl0mydspSIG0[int((65536.0f * fRec5[i]))]))));
				fRec1[i] = (fZec1[i] - std::floor(fZec1[i]));
			}
			/* Post code */
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec1_perm[j11] = fRec1_tmp[(vsize + j11)];
			}
			/* Recursive loop 8 */
			/* Pre code */
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec7_tmp[j14] = fRec7_perm[j14];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec7[i] = (fRec8[i] + (fRec7[(i - 1)] * float((fRec8[(i - 1)] >= fRec8[i]))));
			}
			/* Post code */
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec7_perm[j15] = fRec7_tmp[(vsize + j15)];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec10_tmp[j18] = fRec10_perm[j18];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec10[i] = (fSlow6 + (fConst2 * fRec10[(i - 1)]));
			}
			/* Post code */
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec10_perm[j19] = fRec10_tmp[(vsize + j19)];
			}
			/* Recursive loop 10 */
			/* Pre code */
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec11_tmp[j20] = fRec11_perm[j20];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec11[i] = (fSlow7 + (fConst2 * fRec11[(i - 1)]));
			}
			/* Post code */
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec11_perm[j21] = fRec11_tmp[(vsize + j21)];
			}
			/* Recursive loop 11 */
			/* Pre code */
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				iRec12_tmp[j22] = iRec12_perm[j22];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iRec12[i] = ((fRec8[i] == 0.0f) * (iRec12[(i - 1)] + 1));
			}
			/* Post code */
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				iRec12_perm[j23] = iRec12_tmp[(vsize + j23)];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec2[i] = std::max<float>(1.0f, (fConst0 * fRec9[i]));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = FAUSTFLOAT((0.300000012f * ((fRec0[i] * ftbl0mydspSIG0[int((65536.0f * fRec1[i]))]) * std::max<float>(0.0f, (std::min<float>((fRec7[i] / fZec2[i]), std::max<float>(((((1.0f - fRec10[i]) * (fZec2[i] - fRec7[i])) / std::max<float>(1.0f, (fConst0 * fRec11[i]))) + 1.0f), fRec10[i])) * (1.0f - (fSlow8 * float(iRec12[i]))))))));
			}
		}
		/* Remaining frames */
		if ((vindex < count)) {
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			int vsize = (count - vindex);
			/* Recursive loop 0 */
			/* Pre code */
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec2_tmp[j2] = fRec2_perm[j2];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec2[i] = (fSlow1 + (fConst2 * fRec2[(i - 1)]));
			}
			/* Post code */
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec2_perm[j3] = fRec2_tmp[(vsize + j3)];
			}
			/* Recursive loop 1 */
			/* Pre code */
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec3_tmp[j4] = fRec3_perm[j4];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec3[i] = (fSlow2 + (fConst2 * fRec3[(i - 1)]));
			}
			/* Post code */
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec3_perm[j5] = fRec3_tmp[(vsize + j5)];
			}
			/* Recursive loop 2 */
			/* Pre code */
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				fRec4_tmp[j6] = fRec4_perm[j6];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec4[i] = (fSlow3 + (fConst2 * fRec4[(i - 1)]));
			}
			/* Post code */
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				fRec4_perm[j7] = fRec4_tmp[(vsize + j7)];
			}
			/* Recursive loop 3 */
			/* Pre code */
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec5_tmp[j8] = fRec5_perm[j8];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec0[i] = (fRec5[(i - 1)] + (fConst3 * (fRec2[i] * fRec3[i])));
				fRec5[i] = (fZec0[i] - std::floor(fZec0[i]));
			}
			/* Post code */
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec5_perm[j9] = fRec5_tmp[(vsize + j9)];
			}
			/* Recursive loop 4 */
			/* Pre code */
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				fRec8_tmp[j12] = fRec8_perm[j12];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec8[i] = (fSlow4 + (fConst2 * fRec8[(i - 1)]));
			}
			/* Post code */
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				fRec8_perm[j13] = fRec8_tmp[(vsize + j13)];
			}
			/* Recursive loop 5 */
			/* Pre code */
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fRec9_tmp[j16] = fRec9_perm[j16];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec9[i] = (fSlow5 + (fConst2 * fRec9[(i - 1)]));
			}
			/* Post code */
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fRec9_perm[j17] = fRec9_tmp[(vsize + j17)];
			}
			/* Recursive loop 6 */
			/* Pre code */
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec0_tmp[j0] = fRec0_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec0[i] = (fSlow0 + (fConst2 * fRec0[(i - 1)]));
			}
			/* Post code */
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec0_perm[j1] = fRec0_tmp[(vsize + j1)];
			}
			/* Recursive loop 7 */
			/* Pre code */
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec1_tmp[j10] = fRec1_perm[j10];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec1[i] = (fRec1[(i - 1)] + (fConst3 * (fRec2[i] + ((fRec3[i] * fRec4[i]) * ftbl0mydspSIG0[int((65536.0f * fRec5[i]))]))));
				fRec1[i] = (fZec1[i] - std::floor(fZec1[i]));
			}
			/* Post code */
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec1_perm[j11] = fRec1_tmp[(vsize + j11)];
			}
			/* Recursive loop 8 */
			/* Pre code */
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec7_tmp[j14] = fRec7_perm[j14];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec7[i] = (fRec8[i] + (fRec7[(i - 1)] * float((fRec8[(i - 1)] >= fRec8[i]))));
			}
			/* Post code */
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec7_perm[j15] = fRec7_tmp[(vsize + j15)];
			}
			/* Recursive loop 9 */
			/* Pre code */
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec10_tmp[j18] = fRec10_perm[j18];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec10[i] = (fSlow6 + (fConst2 * fRec10[(i - 1)]));
			}
			/* Post code */
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec10_perm[j19] = fRec10_tmp[(vsize + j19)];
			}
			/* Recursive loop 10 */
			/* Pre code */
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec11_tmp[j20] = fRec11_perm[j20];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec11[i] = (fSlow7 + (fConst2 * fRec11[(i - 1)]));
			}
			/* Post code */
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec11_perm[j21] = fRec11_tmp[(vsize + j21)];
			}
			/* Recursive loop 11 */
			/* Pre code */
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				iRec12_tmp[j22] = iRec12_perm[j22];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iRec12[i] = ((fRec8[i] == 0.0f) * (iRec12[(i - 1)] + 1));
			}
			/* Post code */
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				iRec12_perm[j23] = iRec12_tmp[(vsize + j23)];
			}
			/* Vectorizable loop 12 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec2[i] = std::max<float>(1.0f, (fConst0 * fRec9[i]));
			}
			/* Vectorizable loop 13 */
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				output0[i] = FAUSTFLOAT((0.300000012f * ((fRec0[i] * ftbl0mydspSIG0[int((65536.0f * fRec1[i]))]) * std::max<float>(0.0f, (std::min<float>((fRec7[i] / fZec2[i]), std::max<float>(((((1.0f - fRec10[i]) * (fZec2[i] - fRec7[i])) / std::max<float>(1.0f, (fConst0 * fRec11[i]))) + 1.0f), fRec10[i])) * (1.0f - (fSlow8 * float(iRec12[i]))))))));
			}
		}
	}

};

// END-FAUSTDSP

#endif
