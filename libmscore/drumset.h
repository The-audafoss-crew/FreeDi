//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __DRUMSET_H__
#define __DRUMSET_H__

#include "mscore.h"
#include "tremolo.h"
#include "note.h"
#include "articulation.h"
#include "sym.h"

namespace Ms {

class Xml;

struct DrumInstrumentVariant {
      int pitch;
      QString articulationName;
      TremoloType tremolo;
      DrumInstrumentVariant() {
            pitch = INVALID_PITCH;
            tremolo = TremoloType::INVALID_TREMOLO;
      }
};

//---------------------------------------------------------
//   DrumInstrument
//---------------------------------------------------------

struct DrumInstrument {
      QString name;
      int index = 0;
      int pitch = -1;
      
      // if @notehead == HEAD_CUSTOM, use @noteheads to extract custom noteheads
      NoteHead::Group notehead; ///< notehead symbol set
      SymId noteheads[int(NoteHead::Type::HEAD_TYPES)] = { SymId::noteheadWhole, SymId::noteheadHalf, SymId::noteheadBlack, SymId::noteheadDoubleWhole  };

      int line;                 ///< place notehead onto this line
      MScore::Direction stemDirection;
      int voice;
      char shortcut;            ///< accelerator key (CDEFGAB)
      QList<DrumInstrumentVariant> variants;

      DrumInstrument() {}
      DrumInstrument(const char* s, NoteHead::Group nh, int l, MScore::Direction d,
         int v = 0, char sc = 0)
         : name(s), notehead(nh), line(l), stemDirection(d), voice(v), shortcut(sc) {}
      void addVariant(DrumInstrumentVariant v) { variants.append(v); }
      bool operator<(const DrumInstrument& other) const { return (index != other.index ? index < other.index : pitch < other.pitch); };
      };

static const int DRUM_INSTRUMENTS = 128;

//---------------------------------------------------------
//   Drumset
//    defines noteheads and line position for all
//    possible midi notes in a drumset
//---------------------------------------------------------

class Drumset {
      std::vector<DrumInstrument> _drum;
      
   public:
      Drumset();
      bool isValid(int pitch) const             { return pitch > -1 && pitch < DRUM_INSTRUMENTS && !_drum[pitch].name.isEmpty(); }
      NoteHead::Group noteHead(int pitch) const { return _drum[pitch].notehead;       }
      SymId noteHeads(int pitch, NoteHead::Type t) const  { return _drum[pitch].noteheads[int(t)];      }
      
      int line(int pitch) const                 { return _drum[pitch].line;           }
      int voice(int pitch) const                { return _drum[pitch].voice;          }
      MScore::Direction stemDirection(int pitch) const  { return _drum[pitch].stemDirection;  }
      const QString& name(int pitch) const      { return _drum[pitch].name;           }
      int shortcut(int pitch) const             { return _drum[pitch].shortcut;       }
      QList<DrumInstrumentVariant> variants(int pitch) const   { return _drum[pitch].variants; }

      std::vector<DrumInstrument> drumsByIndex() const;
      void save(Xml&) const;
      void load(XmlReader&, int index);
      void clear();
      int nextPitch(int) const;
      int prevPitch(int) const;
      DrumInstrument& drum(int i) { return _drum[i]; }
      const DrumInstrument& drum(int i) const { return _drum[i]; }
      void addDrumInstrument(int pitch, DrumInstrument d) { d.pitch = pitch; _drum[pitch] = d; };
      DrumInstrumentVariant findVariant(int pitch, const QList<Articulation*> articulations, Tremolo* tremolo) const;
      };

extern Drumset* smDrumset;
extern Drumset* gpDrumset;
extern void initDrumset();


}     // namespace Ms
#endif

