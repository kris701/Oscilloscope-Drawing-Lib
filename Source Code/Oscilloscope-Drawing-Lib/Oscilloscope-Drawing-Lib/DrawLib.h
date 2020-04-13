#pragma once

typedef struct
{
    int X;
    int Y;
} Point;

typedef struct
{
    Point From;
    Point To;
} Path;

enum DrawQuality { DQ_Low, DQ_Medium, DQ_High };

// Font Data
static constexpr Path Blank[] PROGMEM = { {{0,0},{0,0}} };
static constexpr Path Letter_A[] PROGMEM = { {{0,0},{256,512}}, {{256,512},{512,0}},{{384,256},{128,256}} };
static constexpr Path Letter_B[] PROGMEM = { {{150,0},{150,512}},{{150,512},{225,480}},{{225,480},{256,406}} ,{{256,406},{225,331}}, {{225,331},{150, 300}}, {{150, 300},{256,256}}, {{256,256},{300,150}},{{300,150},{256,44}}, {{256,44},{150,0}} };
static constexpr Path Letter_C[] PROGMEM = { {{362,0},{181,75}},{{181,75},{106,256}},{{106,256},{181,437}} ,{{181,437},{362,512}} };

static constexpr Path Number_Zero[] PROGMEM = { {{0,0},{256,0}}, {{256,0},{256,512}},{{256,512},{0,512}},{{0,512},{0,0}} };
static constexpr Path Number_One[] PROGMEM = { {{0,0},{0,512}} };
static constexpr Path Number_Two[] PROGMEM = { {{256,0},{0,0}}, {{0,0},{0,256}},{{0,256},{256,256}},{{256,256},{256,512}}, {{256,512},{0,512}} };
static constexpr Path Number_Three[] PROGMEM = { {{0,0},{256,0}}, {{256,0},{256,512}},{{256,512}, {0,512}}, {{256,256},{0,256}} };
static constexpr Path Number_Four[] PROGMEM = { {{256,0},{256,512}}, {{256,256},{0,256}}, {{0,256},{0,512}} };
static constexpr Path Number_Five[] PROGMEM = { {{0,0},{256,0}}, {{256,0},{256,256}},{{256,256},{0,256}}, {{0,256},{0,512}}, {{0,512},{256, 512}} };
static constexpr Path Number_Six[] PROGMEM = { {{256,512},{0,512}}, {{0,512},{0,0}},{{0,0},{256,0}}, {{256,0},{256,256}}, {{256,256},{0, 256}} };
static constexpr Path Number_Seven[] PROGMEM = { {{256,0},{256,512}}, {{256,512},{0,512}} };
static constexpr Path Number_Eight[] PROGMEM = { {{0,0},{256,0}}, {{256,0},{256,512}},{{256,512},{0,512}}, {{0,512},{0,0}}, {{256,256}, {0, 256}} };
static constexpr Path Number_Nine[] PROGMEM = { {{256,0},{256,512}}, {{256,512},{0,512}},{{0,512},{0,256}}, {{0,256},{256,256}} };

class fontClass
{
private:


public:
    static const Path* GetLetterTrace(char Letter, int* Size)
    {
        switch (Letter)
        {
        case 'A':
            *Size = sizeof(Letter_A) / sizeof(Path);
            return Letter_A;
        case 'B':
            *Size = sizeof(Letter_B) / sizeof(Path);
            return Letter_B;
        case 'C':
            *Size = sizeof(Letter_C) / sizeof(Path);
            return Letter_C;

        case '0':
            *Size = sizeof(Number_Zero) / sizeof(Path);
            return Number_Zero;
        case '1':
            *Size = sizeof(Number_One) / sizeof(Path);
            return Number_One;
        case '2':
            *Size = sizeof(Number_Two) / sizeof(Path);
            return Number_Two;
        case '3':
            *Size = sizeof(Number_Three) / sizeof(Path);
            return Number_Three;
        case '4':
            *Size = sizeof(Number_Four) / sizeof(Path);
            return Number_Four;
        case '5':
            *Size = sizeof(Number_Five) / sizeof(Path);
            return Number_Five;
        case '6':
            *Size = sizeof(Number_Six) / sizeof(Path);
            return Number_Six;
        case '7':
            *Size = sizeof(Number_Seven) / sizeof(Path);
            return Number_Seven;
        case '8':
            *Size = sizeof(Number_Eight) / sizeof(Path);
            return Number_Eight;
        case '9':
            *Size = sizeof(Number_Nine) / sizeof(Path);
            return Number_Nine;
        }
        *Size = 1;
        return Blank;
    }
};

class drawClass
{
private: double JumpFactorA;
private: int JumpFactorB;
private: int MaxJump;
private: int MinStep;
private: int CircleRes;
private: int MaxFillLines;

private: int XAxis;
private: int YAxis;
private: int XAxisWidth;
private: int YAxisWidth;

public:
    void Config(int XPin, int YPin, int XWidth, int YHeight, int AnalogRes)
    {
        analogWriteResolution(AnalogRes);

        XAxis = XPin;
        YAxis = YPin;
        XAxisWidth = XWidth;
        YAxisWidth = YHeight;

        pinMode(XAxis, OUTPUT);
        pinMode(YAxis, OUTPUT);

        SetQuality(DQ_Medium);
    }

    void SetQuality(DrawQuality Quality)
    {
        switch (Quality)
        {
        case DQ_Low:
            JumpFactorA = 0.05;
            JumpFactorB = 2;
            MaxJump = 50;
            MinStep = 15;
            CircleRes = 12;
            MaxFillLines = 50;
            break;
        case DQ_Medium:
            JumpFactorA = 0.1;
            JumpFactorB = 20;
            MaxJump = 150;
            MinStep = 10;
            CircleRes = 24;
            MaxFillLines = 150;
            break;
        case DQ_High:
            JumpFactorA = 0.15;
            JumpFactorB = 30;
            MaxJump = 250;
            MinStep = 5;
            CircleRes = 50;
            MaxFillLines = 250;
            break;
        }
    }

    void SetQuality(double _JumpFactorA, int _JumpFactorB, int _MaxJump, int _MinStep)
    {
        JumpFactorA = _JumpFactorA;
        JumpFactorB = _JumpFactorB;
        MaxJump = _MaxJump;
        MinStep = _MinStep;
    }

    void DrawString(String Text, Point RelativeOffset, double Scale, int Rotation)
    {
        for (int i = 0; i < Text.length(); i++)
        {
            int SizeOfTargetLetterArray;
            const Path* TargetLetter = fontClass::GetLetterTrace(Text[i], &SizeOfTargetLetterArray);

            DrawPath(TargetLetter, SizeOfTargetLetterArray, RelativeOffset, Scale, Rotation);

            RelativeOffset.X += 512 * Scale;
        }
    }

    void DrawLetter(char Letter, Point RelativeOffset, double Scale, int Rotation)
    {
        int SizeOfTargetLetterArray;
        const Path* TargetLetter = fontClass::GetLetterTrace(Letter, &SizeOfTargetLetterArray);

        DrawPath(TargetLetter, SizeOfTargetLetterArray, RelativeOffset, Scale, Rotation);
    }

    void DrawPath(const Path* PathArray, int Size, Point Relative, double Scale, int Rotation)
    {
        Point FromPoint = { 0,0 };
        Point ToPoint = { 0,0 };
        Point Center = GetOrigoOfPath(PathArray, Size);

        for (int i = 0; i < Size; i++)
        {
            FromPoint = TransformPoint(PathArray[i].From, Relative, Rotation, Scale, Center);
            ToPoint = TransformPoint(PathArray[i].To, Relative, Rotation, Scale, Center);

            TraceTo(ToPoint, FromPoint);
        }
    }

    void DrawRegtangle(Point Position, int Width, int Height, double Scale, int Rotation, bool Fill)
    {
        Path NewReg[4] = {};
        NewReg[0].From.X = 0;
        NewReg[0].From.Y = 0;
        NewReg[0].To.X = Width;
        NewReg[0].To.Y = 0;

        NewReg[1].From.X = NewReg[0].To.X;
        NewReg[1].From.Y = NewReg[0].To.Y;
        NewReg[1].To.X = Width;
        NewReg[1].To.Y = Height;

        NewReg[2].From.X = NewReg[1].To.X;
        NewReg[2].From.Y = NewReg[1].To.Y;
        NewReg[2].To.X = 0;
        NewReg[2].To.Y = Height;

        NewReg[3].From.X = NewReg[2].To.X;
        NewReg[3].From.Y = NewReg[2].To.Y;
        NewReg[3].To.X = 0;
        NewReg[3].To.Y = 0;

        DrawPath(NewReg, 4, Position, Scale, Rotation);

        if (Fill)
        {
            Path FillReg[250] = {};
            for (int i = 0; i < MaxFillLines; i++)
            {
                FillReg[i].From = { 0, (Height / MaxFillLines) * (i + 1) };
                FillReg[i].To = { Width, (Height / MaxFillLines) * (i + 1) };
            }

            DrawPath(FillReg, MaxFillLines, Position, Scale, Rotation);
        }
    }

    void DrawCircle(Point Center, int Radius, double Scale)
    {
        Path NewReg[50] = {};
        for (int i = 0; i < CircleRes; i++)
        {
            if (i == 0)
                NewReg[i].From = { Center.X + Radius , Center.Y };
            else
                NewReg[i].From = NewReg[i - 1].To;

            NewReg[i].To = RotatePoint({ Center.X + Radius , Center.Y }, Center, (360 / CircleRes) * (i + 1));
        }

        DrawPath(NewReg, CircleRes, { 0,0 }, Scale, 0);
    }

    void DrawLine(Point From, Point To, double Scale, int Rotation)
    {
        Path NewLine[1] = { From, To };
        DrawPath(NewLine, 1, { 0,0 }, Scale, Rotation);
    }

private:
    Point GetOrigoOfPath(const Path* SenderPath, int Size)
    {
        int MaxX = 0;
        int MaxY = 0;
        int MinX = 99999999999999;
        int MinY = 99999999999999;
        for (int i = 0; i < Size; i++)
        {
            if (SenderPath[i].From.X > MaxX)
                MaxX = SenderPath[i].From.X;
            if (SenderPath[i].From.Y > MaxY)
                MaxY = SenderPath[i].From.Y;

            if (SenderPath[i].From.X < MinX)
                MinX = SenderPath[i].From.X;
            if (SenderPath[i].From.Y < MinY)
                MinY = SenderPath[i].From.Y;

            if (SenderPath[i].To.X > MaxX)
                MaxX = SenderPath[i].To.X;
            if (SenderPath[i].To.Y > MaxY)
                MaxY = SenderPath[i].To.Y;

            if (SenderPath[i].To.X < MinX)
                MinX = SenderPath[i].To.X;
            if (SenderPath[i].To.Y < MinY)
                MinY = SenderPath[i].To.Y;
        }

        Point ReturnPoint = { 0,0 };
        ReturnPoint.X = (MaxX - MinX) / 2;
        ReturnPoint.Y = (MaxY - MinY) / 2;
        return ReturnPoint;
    }

    int CalculateDistance(Point Loc, Point CurrentPoint)
    {
        return sqrt(pow(Loc.X - CurrentPoint.X, 2) + pow(Loc.Y - CurrentPoint.Y, 2));
    }

    Point TransformPoint(Point SenderPoint, Point Relative, int Rotation, double Scale, Point Oriogo)
    {
        SenderPoint.X = (SenderPoint.X * Scale) + Relative.X;
        SenderPoint.Y = (SenderPoint.Y * Scale) + Relative.Y;

        if (Rotation != 0)
        {
            Relative.X += Oriogo.X;
            Relative.Y += Oriogo.Y;
            SenderPoint = RotatePoint(SenderPoint, Relative, Rotation);
        }

        return SenderPoint;
    }

    Point RotatePoint(Point ToRotate, Point AroundPoint, int Rotation)
    {
        float s = sin(DegreesToRadians(Rotation));
        float c = cos(DegreesToRadians(Rotation));

        // translate point back to origin:
        ToRotate.X -= AroundPoint.X;
        ToRotate.Y -= AroundPoint.Y;

        // rotate point
        float xnew = ToRotate.X * c - ToRotate.Y * s;
        float ynew = ToRotate.X * s + ToRotate.Y * c;

        // translate point back:
        ToRotate.X = xnew + AroundPoint.X;
        ToRotate.Y = ynew + AroundPoint.Y;
        return ToRotate;
    }

    double DegreesToRadians(int Degree)
    {
        return Degree * PI / 180;
    }

    int GetJumpSteps(int AbsDiff)
    {
        int Factor = AbsDiff * JumpFactorA + JumpFactorB;
        if (Factor == 0)
            Factor = 1;
        if (Factor > MaxJump)
            Factor = MaxJump;
        //Serial.println("JF: " + String(Factor) + " DIFF: " + String(AbsDiff));
        return Factor;
    }

    void TraceTo(Point Loc, Point CurrentPoint)
    {
        if (Loc.X == CurrentPoint.X && Loc.Y == CurrentPoint.Y)
        {
            analogWrite(XAxis, CurrentPoint.X);
            analogWrite(YAxis, CurrentPoint.Y);
            return;
        }

        int JumpStep = GetJumpSteps(CalculateDistance(Loc, CurrentPoint));
        float XJump = (Loc.X - CurrentPoint.X) / JumpStep;
        float YJump = (Loc.Y - CurrentPoint.Y) / JumpStep;
        if (XJump == 0 && (Loc.X - CurrentPoint.X) != 0)
        {
            if (Loc.X < CurrentPoint.X)
                XJump = -1;
            if (Loc.X > CurrentPoint.X)
                XJump = 1;
        }
        if (YJump == 0 && (Loc.Y - CurrentPoint.Y) != 0)
        {
            if (Loc.Y < CurrentPoint.Y)
                YJump = -1;
            if (Loc.Y > CurrentPoint.Y)
                YJump = 1;
        }

        //Serial.println("XF: " + String(XJump));
        //Serial.println("YF: " + String(YJump));

        while (Loc.X != CurrentPoint.X || Loc.Y != CurrentPoint.Y)
        {
            //Serial.println("JS: " + String(JumpStep));

            //Serial.println("XJ: " + String(XJump));
            //Serial.println("YJ: " + String(YJump));

            //Serial.println("CLX: " + String(CurrentPoint.X) + " CLY: " + String(CurrentPoint.Y));
            //Serial.println("TLX: " + String(Loc.X) + " TLY: " + String(Loc.Y));

            if (Loc.X != CurrentPoint.X)
            {
                CurrentPoint.X += XJump;

                if (CurrentPoint.X < 0)
                    CurrentPoint.X = Loc.X;
                if (CurrentPoint.X > XAxisWidth)
                    CurrentPoint.X = Loc.X;
                if (abs(Loc.X - CurrentPoint.X) < MinStep)
                    CurrentPoint.X = Loc.X;
            }

            if (Loc.Y != CurrentPoint.Y)
            {
                CurrentPoint.Y += YJump;

                if (CurrentPoint.Y < 0)
                    CurrentPoint.Y = Loc.Y;
                if (CurrentPoint.Y > YAxisWidth)
                    CurrentPoint.Y = Loc.Y;
                if (abs(Loc.Y - CurrentPoint.Y) < MinStep)
                    CurrentPoint.Y = Loc.Y;
            }

            analogWrite(XAxis, CurrentPoint.X);
            analogWrite(YAxis, CurrentPoint.Y);
        }

        analogWrite(XAxis, CurrentPoint.X);
        analogWrite(YAxis, CurrentPoint.Y);
    }
};