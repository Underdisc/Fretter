
#include <iostream>
#include <ostream>
#include <vector>

#define NotesPerOctave 12

void ChangeColor(std::ostream & os, const char * new_color)
{
    os << new_color;
}

class Note
{
public:
    enum Types
    {
        A = 0,
        ASharp = 1,
        BFlat = 1,
        B = 2,
        C = 3,
        CSharp = 4,
        DFlat = 4,
        D = 5,
        DSharp = 6,
        Eflat = 6,
        E = 7,
        F = 8,
        FSharp = 9,
        GFlat = 9,
        G = 10,
        GSharp = 11,
        AFlat = 11
    };
    int _type;
    int _octave;
    Note();
    Note(int type, int octave);
    Note(const Note & other);
    int NoteStringLength() const;
    bool EqualType(const Note & other) const;
    Note operator+(int interval) const;
    Note operator-(int interval) const;
    bool operator==(const Note & other) const;
    bool operator!=(const Note & other) const;
    friend std::ostream & operator<<(std::ostream &, const Note &);

    static const std::string note_types[NotesPerOctave];


};

const std::string Note::note_types[NotesPerOctave] = { "A",
                                                       "A#:Bb",
                                                       "B",
                                                       "C",
                                                       "C#:Db",
                                                       "D",
                                                       "D#:Eb",
                                                       "E",
                                                       "F",
                                                       "F#:Gb",
                                                       "G",
                                                       "G#:Ab" };


Note::Note()
{
}

Note::Note(int type, int octave) : _type(type), _octave(octave)
{
}

Note::Note(const Note & other) : _type(other._type), _octave(other._octave)
{
}

int Note::NoteStringLength() const
{
    return note_types[_type].size();
}

bool Note::EqualType(const Note & other) const
{
    return _type == other._type;
}

Note Note::operator+(int interval) const
{
    Note new_note;
    int new_type = _type + interval;
    int octave_shift = new_type / NotesPerOctave;
    if(new_type >= 0)
    {
        new_note._type = new_type % NotesPerOctave;
    }
    else
    {
        octave_shift -= 1;
        new_note._type = NotesPerOctave - (-new_type % NotesPerOctave);
    }
    new_note._octave = _octave + octave_shift;
    return new_note;
}

Note Note::operator-(int interval) const
{
    Note new_note = (*this) + -interval;
    return new_note;
}

bool Note::operator==(const Note & other) const
{
    return _type == other._type && _octave == other._octave;
}

bool Note::operator!=(const Note & other) const
{
    return !(*this == other);
}

std::ostream & operator<<(std::ostream & os, const Note & note)
{
    if(note._type < 0 || note._type >= NotesPerOctave)
    {
        os << "Not a note type";
        return os;
    }
    os << Note::note_types[note._type];
    return os; 
}

class NoteTest
{
public:
    static bool Test();
    static bool TestOperators();
};

bool NoteTest::Test()
{
    return TestOperators();
}

bool NoteTest::TestOperators()
{
    std::vector<Note> test_notes;
    test_notes.push_back(Note(Note::A, 0));
    test_notes.push_back(Note(test_notes[0] + 1));
    test_notes.push_back(Note(test_notes[1] + 1));
    test_notes.push_back(Note(test_notes[2] + 12));
    test_notes.push_back(Note(test_notes[3] + 3));
    test_notes.push_back(Note(test_notes[4] + 10));
    test_notes.push_back(Note(test_notes[5] - 10));
    test_notes.push_back(Note(test_notes[6] - 5));

    std::vector<Note> correct_notes;
    correct_notes.push_back(Note(Note::A, 0));
    correct_notes.push_back(Note(Note::ASharp, 0));
    correct_notes.push_back(Note(Note::B, 0));
    correct_notes.push_back(Note(Note::B, 1));
    correct_notes.push_back(Note(Note::D, 1));
    correct_notes.push_back(Note(Note::C, 2));
    correct_notes.push_back(Note(Note::D, 1));
    correct_notes.push_back(Note(Note::A, 1));

    for(int i = 0; i < test_notes.size(); i++)
    {
        if(test_notes[i] != correct_notes[i])
        {
            return false;
        }
    }
    return true;
}

class Scale
{
public:
    Scale(const std::vector<Note> & scale_notes);
    bool InScale(const Note & note) const;
    std::vector<Note> _scale_notes;
};

Scale::Scale(const std::vector<Note> & scale_notes) : _scale_notes(scale_notes)
{}

bool Scale::InScale(const Note & note) const
{
    for(const Note & scale_note : _scale_notes)
    {
        if(note.EqualType(scale_note))
        {
            return true;
        }
    }
    return false;
}

class FretBoard
{
public:
    std::vector<std::vector<Note>> _strings;
    void Initialize(const std::vector<Note> & open_notes, int num_frets);
    void Print(std::ostream & os, int fret_width);
    void PrintScaleHighlighted(std::ostream & os,
                               int fred_width, 
                               const Scale & scale);
private:
};

void FretBoard::Initialize(const std::vector<Note> & open_notes, int num_frets)
{
    _strings.reserve(open_notes.size());
    for(int i = 0; i < open_notes.size(); ++i)
    {
        _strings.push_back(std::vector<Note>());
        const Note & open_note = open_notes[i];
        _strings[i].push_back(open_note);
        for(int j = 1; j < num_frets; ++j)
        {
            _strings[i].push_back(open_note + j);
        }
    }
}

void FretBoard::Print(std::ostream & os, int fret_width)
{
    for(int s = 0; s < _strings.size(); ++s)
    {
        const std::vector<Note> & string = _strings[s];
        for(int n = 0; n < string.size(); ++n)
        {
            os << "|-" << string[n];
            int char_remaining = fret_width - 2 - string[n].NoteStringLength();
            for(int t = 0; t < char_remaining; ++t)
            {
                os << "-";
            }
        }
        os << std::endl;
    }
}

void FretBoard::PrintScaleHighlighted(std::ostream & os,
                                      int fret_width,
                                      const Scale & scale)
{
    for(int s = 0; s < _strings.size(); ++s)
    {
        const std::vector<Note> & string = _strings[s];
        for(int n = 0; n < string.size(); ++n)
        {
            os << "|";
            const Note & note = string[n];
            if(scale.InScale(note))
            {
                ChangeColor(std::cout, "\033[31;1m");
            }
            os << "-" << string[n];
            int char_remaining = fret_width - 1 - string[n].NoteStringLength();
            for(int t = 0; t < char_remaining; ++t)
            {
                os << "-";
            }
            ChangeColor(std::cout,"\033[0m");
        }
        os << std::endl;
    }
}


int main(void)
{
    // Initialize and print the FretBoard.
    std::vector<Note> open_strings;
    open_strings.push_back(Note(Note::E, 4));
    open_strings.push_back(Note(Note::B, 4));
    open_strings.push_back(Note(Note::G, 3));
    open_strings.push_back(Note(Note::D, 3));
    open_strings.push_back(Note(Note::A, 3));
    open_strings.push_back(Note(Note::E, 2));
    FretBoard fret_board;
    fret_board.Initialize(open_strings, 12);

    // Create the C Major Scale.
    std::vector<Note> c_major_notes;
    c_major_notes.push_back(Note(Note::C, 0));
    c_major_notes.push_back(Note(Note::D, 0));
    c_major_notes.push_back(Note(Note::E, 0));
    c_major_notes.push_back(Note(Note::F, 0));
    c_major_notes.push_back(Note(Note::G, 0));
    c_major_notes.push_back(Note(Note::A, 0));
    c_major_notes.push_back(Note(Note::B, 0));

    Scale c_major(c_major_notes);

    fret_board.PrintScaleHighlighted(std::cout, 7, c_major);
    return 0;
}
