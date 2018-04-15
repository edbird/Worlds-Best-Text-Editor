#ifndef COLOR_PALETTE_HPP
#define COLOR_PALETTE_HPP


#include <SDL.h>


#include <vector>
#include <memory>


// list of all different editor object types / colors which are associated
// with an editor object
enum class ColorType
{
    BACKGROUND,
    FOREGROUND,
    TEXT_DEFAULT,
    CURRENT_LINE_BACKGROUND
};


class ColorPalette
{

    public:

    ColorPalette()
    {
        // initialize default colors
        // required or program will crash when calling "Get"
        init_default();
    }

    SDL_Color Get(const std::size_t index) const
    {
        return *_color_.at(index);
    }

    SDL_Color Get(const std::string& name) const
    {
        return *_color_map_name_.at(name);
    }

    SDL_Color Get(const ColorType color_type) const
    {
        return *_color_map_type_.at(color_type);
    }


    private:


    // add new color to the color vector (numerical order) and color map
    // (named colors)
    //void add_color(const Uint8 r, const Uint8 g, const Uint8 b, const std::string& name, const ColorType color_type)
    void add_color(const Uint8 r, const Uint8 g, const Uint8 b, const std::string& name)
    {
        std::shared_ptr<SDL_Color> color_shared_ptr(new SDL_Color{r, g, b});
        _color_.push_back(color_shared_ptr);
        _color_map_name_.insert({name, color_shared_ptr});
        //_color_map_type_.insert({color_type, color_shared_ptr});
    }

    // map the color type object to color using color name (string)
    void map_color(const ColorType color_type, const std::string& name)
    {
        ColorMapNameIterator_t it{_color_map_name_.find(name)};
        if(it != _color_map_name_.end())
        {
            // store a copy of the smart pointer object in the map
            _color_map_type_.insert({color_type, it->second});
        }
        else
        {
            std::cerr << "Error in " << __func__ << " : Color with name " << name << " not found in map _color_map_name_" << std::endl;
            throw "Error";
        }
    }

    void init_default()
    {
        ////////////////////////////////////////////////////////////////////////
        // Group 0
        ////////////////////////////////////////////////////////////////////////

        // 0 = black
        add_color(0x00, 0x00, 0x00, "black");
        
        // 1 = white
        add_color(0xFF, 0xFF, 0xFF, "white");


        ////////////////////////////////////////////////////////////////////////
        // Group 1
        ////////////////////////////////////////////////////////////////////////

        // 2 = red
        add_color(0xFF, 0x00, 0x00, "red");
        
        // 3 = green
        add_color(0x00, 0xFF, 0x00, "green");
        
        // 4 = blue
        add_color(0x00, 0x00, 0xFF, "blue");
        

        ////////////////////////////////////////////////////////////////////////
        // Group 2
        ////////////////////////////////////////////////////////////////////////
        
        // 5 = yellow
        add_color(0xFF, 0xFF, 0x00, "yellow");
        
        // 6 = cyan
        add_color(0x00, 0xFF, 0xFF, "cyan");
        
        // 7 = purple
        add_color(0xFF, 0x00, 0xFF, "purple");
        

        ////////////////////////////////////////////////////////////////////////
        // Group 3
        ////////////////////////////////////////////////////////////////////////
        
        // 8 = dark red
        add_color(0x88, 0x00, 0x00, "darkred");
        
        // 9 = dark green
        add_color(0x00, 0x88, 0x00, "darkgreen");
        
        // 10 = dark blue
        add_color(0x00, 0x00, 0x88, "darkblue");

        // 11 = grey
        add_color(0x88, 0x88, 0x88, "grey");

        // 12 = orange
        add_color(0xFF, 0x88, 0x00, "orange");
         
        // 13 = yellow-green (TODO change name)
        add_color(0x88, 0xFF, 0x00, "yellow-green");
 
        // 14 = green-cyan
        add_color(0x00, 0xFF, 0x88, "green-cyan");
        
        // 15 = blue-cyan
        add_color(0x00, 0x88, 0xFF, "blue-cyan");
        
        // 16 = pink
        add_color(0xFF, 0x00, 0x88, "pink");
        
        // 14 = violet
        add_color(0x88, 0x00, 0xFF, "violet");
        
        ////////////////////////////////////////////////////////////////////////
        // Group 4
        ////////////////////////////////////////////////////////////////////////
        


        // map all colors
        map_color(ColorType::BACKGROUND, "white");
        map_color(ColorType::FOREGROUND, "black");
        map_color(ColorType::TEXT_DEFAULT, "black");
        map_color(ColorType::CURRENT_LINE_BACKGROUND, "grey");

    }

    // store all colors in numerical order
    std::vector<std::shared_ptr<SDL_Color>> _color_;
    typedef std::vector<std::shared_ptr<SDL_Color>>::iterator ColorIterator_t;

    // store all colors with associated name
    std::map<std::string, std::shared_ptr<SDL_Color>> _color_map_name_;
    typedef std::map<std::string, std::shared_ptr<SDL_Color>>::iterator ColorMapNameIterator_t;
    
    // store all colors with associated ColorType
    std::map<ColorType, std::shared_ptr<SDL_Color>> _color_map_type_;
    typedef std::map<ColorType, std::shared_ptr<SDL_Color>>::iterator ColorMapTypeIterator_t;


};

#endif // COLOR_PALETTE_HPP
