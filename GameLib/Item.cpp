/**
 * @file Item.cpp
 * @author Haoxiang Zhang, Shaojie Zhang
 */

#include "pch.h"
#include "Item.h"
#include "Stadium.h"

using namespace std;

/**
 * Constructor
 * @param stadium  The stadium this item is member of
 * @param picture The picture to draw
 */
Item::Item(Stadium *stadium, std::shared_ptr<Picture> picture) : mStadium(stadium), mPicture(picture)
{

}

/**
 * Destructor
 */
Item::~Item()
{

}


void Item::Update(double elapsed)
{
//  Vector mV;
//  Vector newP = GetPos() + mV*elapsed;
//  SetLocation(newP);
}

wxXmlNode* Item::XmlSave(wxXmlNode* node)
{
    auto itemNode = new wxXmlNode(wxXML_ELEMENT_NODE, L"id");
    node->AddChild(itemNode);

    itemNode->AddAttribute(L"x", wxString::Format(L"%i", mPos.X()));
    itemNode->AddAttribute(L"y", wxString::Format(L"%i", mPos.Y()));

    return itemNode;
}

void Item::XmlLoad(wxXmlNode* node)
{
    long x, y = 0;
    wstring filename;
    node->GetAttribute(L"x", L"0").ToLong(&x);
    node->GetAttribute(L"y", L"0").ToLong(&y);
    mPos.Set(x,y);
}

void Item::Draw(std::shared_ptr<wxGraphicsContext> graphics, int scrollx)
{
    int wid = GetPicture()->GetWidth();
    int hit = GetPicture()->GetHeight();
    graphics->DrawBitmap(GetPicture()->AsBitmap(graphics),
            (int)GetX() - wid / 2 + scrollx, (int)GetY() - hit / 2,
            wid + 1, hit);
}

void Item::SetPicture(const wstring& file)
{
    mPicture->SetImage(file);
}



bool Item::HitTest(int x, int y)
{
    double wid = GetPicture()->GetWidth();
    double hit = GetPicture()->GetHeight();

    double testX = x - GetX() + wid / 2;
    double testY = y - GetY() + hit / 2;

    if (testX < 0 || testY < 0 || testX >= wid || testY >= hit)
    {
        return false;
    }

    return !GetPicture()->GetImage()->IsTransparent((int)testX, (int)testY);
}
