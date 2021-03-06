/**
 * @file Wall.h
 * @author Haoxiang Zhang SHOIMYA CHOWDHURY
 *
 *
 */

#ifndef SPARTYGNOME_WALL_H
#define SPARTYGNOME_WALL_H


#include "ItemPlatform.h"
#include "Visitor.h"

/// Wall class
class Wall : public ItemPlatform{
private:

public:
    /**
     * Constructor
     * @param Stadium The stadium
     * @param picture The picture
     */
    Wall(Stadium *Stadium, std::shared_ptr<Picture> picture);

/// Destructor
    ~Wall();

    /**
     * The draw function
     * @param graphics The graphiphcs we are using
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

    void XmlLoady(wxXmlNode *node, double yPos);

};

#endif //SPARTYGNOME_WALL_H
