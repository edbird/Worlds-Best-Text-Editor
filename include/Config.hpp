#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <fstream>
#include <iostream>
#include <map>



class Config;


template<typename T>
class ConfigOption
{

    friend class Config;


    public:

    ConfigOption(const std::string& name, const T& default_value, const std::string& default_value_string)
        : _option_{default_value}
        , _default_{default_value}
        , _name_{name}
        , _string_default_{default_value_string}
    {
    }


    const T& Get() const
    {
        return _option_;
    }

    const T& GetDefault() const
    {
        return _default_;
    }

    const std::string& GetStringDefault() const
    {
        return _string_default_;
    }

    const std::string& GetName() const
    {
        return _name_;
    }


    void Set(const T& value)
    {
        _option_ = value;
    }

    void SetDefault()
    {
        _option_ = _default_;
    }


    private:

    T _option_; // this is the current value
    T _default_; // this is the default value
    std::string _name_; // this is the name of the option (self-aware)
    std::string _string_default_;   // this is the default value as a string,
                                    // including the default command to set
                                    // the option and any associated comment
};


class Config
{

    public:

    Config()
    {
        initialize();
        //set_defaults();
    
        std::ifstream ifile(_filename_.c_str());
        if(ifile.is_open())
        {
            std::string line;
            std::size_t line_number{0};
            while(std::getline(ifile, line))
            {
                // remove comments from line
                line = chop_comment(line);
               
                // ignore a blank line
                if(line.size() == 0)
                {
                    continue;
                }

                //std::cout << line << std::endl;
                std::string substring;
                std::string command{read_command(line, substring)};

                if(command == std::string("set"))
                {
                    // read the name of the config option following "set"
                    std::string next_substring;
                    std::string name{read_command(substring, next_substring)};

                    // find name
                    // TODO: now have problem where name could occur in multiple option types! (multiple maps)
                    bool found{false};
                    ConfigIterator_t it{_config_.find(name)};
                    if(it != _config_.end())
                    {
                        found = true;

                        // read value
                        int value{read_value_int(next_substring)};

                        it->second.Set(value);

                        std::cout << "property " << name << " set to value " << value << std::endl;

                    //if(name == std::string("fontsize"))
                    //{

                    //}
                    //else if(name == std::string("cursorblinkrate"))
                    //{

                    }

                    if(found == false)
                    {
                        ConfigIntOptionMapIterator_t it{_config_int_option_map_.find(name)};
                        if(it != _config_int_option_map_.end())
                        {
                            found = true;

                            // read value
                            int value{read_value_int(next_substring)};
                            it->second.Set(value);
                            std::cout << "property " << name << " set to value " << value << " (int)" << std::endl;
                        }
                    }
                    
                    if(found == false)
                    {
                        ConfigFloatOptionMapIterator_t it{_config_float_option_map_.find(name)};
                        if(it != _config_float_option_map_.end())
                        {
                            found = true;

                            // read value
                            double value{read_value_double(next_substring)};
                            it->second.Set(value);
                            std::cout << "property " << name << " set to value " << value << " (double)" << std::endl;
                        }
                    }

                    if(found == false)
                    {
                        std::cerr << "Error in config file " << _filename_ << " line " << line_number << " unrecognized name " << name << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Error in config file " << _filename_ << " line " << line_number << " unrecognized command " << command << std::endl;
                }
            
                ++ line_number;
            }
        }
        else
        {
            std::cerr << "Config file " << _filename_ << " does not exist" << std::endl;
            // TODO: create default config file function?
            // Callable function to create default config file
        }
    }

    /*
    int Get(const std::string& name) const
    {
        return _config_.at(name).Get();
    }
    */

    // New code
    int GetInt(const std::string& name) const
    {
        return _config_int_option_map_.at(name).Get();
    }

    // New code
    double GetFloat(const std::string& name) const
    {
        return _config_float_option_map_.at(name).Get();
    }

    private:

    int read_value_int(const std::string& line)
    {
        return std::stoi(line);
    }

    int read_value_double(const std::string& line)
    {
        return std::stoi(line);
    }

    std::string read_command(const std::string& line, std::string& substring)
    {
        std::size_t pos{line.find(' ')};
        substring = line.substr(pos + 1, std::string::npos);
        return line.substr(0, pos);
        // TODO: what happens if it doesn't find? return whole string?
    }

    std::string chop_comment(std::string line)
    {
        if(line.find('#') != std::string::npos)
        {
            return line.substr(0, line.find('#'));
        }
        return line;
    }

    void initialize()
    {
        //_config_.insert(std::map<const std::string, ConfigOption<int>>::value_type("fontsize", ConfigOption<int>("fontsize", 11, "set fontsize 11")));
        //_config_.insert(std::map<const std::string, ConfigOption<int>>::value_type("cursorblinkrate", ConfigOption<int>("cursorblinkrate", 500, "set cursorblinkrate 500 # milliseconds")));

        // TODO: name appears twice!
        _config_int_option_map_.insert(std::map<const std::string, ConfigOption<int>>::value_type("fontsize", ConfigOption<int>("fontsize", 11, "set fontsize 11")));
        _config_int_option_map_.insert(std::map<const std::string, ConfigOption<int>>::value_type("cursorblinkrate", ConfigOption<int>("cursorblinkrate", 500, "set cursorblinkrate 500 # milliseconds")));
        _config_int_option_map_.insert(std::map<const std::string, ConfigOption<int>>::value_type("linenumber", ConfigOption<int>("linenumber", 0, "set linenumber 0 # false (off)")));
        //_config_float_option_map_.insert(std::map<const std::string, ConfigOption<double>>::value_type("")); // double not float
        

    }


    public:


    // TODO: test
    void WriteDefaultConfigFile()
    {
        std::ifstream ifile(_filename_);
        if(ifile.is_open())
        {
            std::cout << "Error: Config file " << _filename_ << " exists" << std::endl;
            ifile.close();
        }
        else
        {
            std::ofstream ofile(_filename_);
            ofile << get_default_config_file();
            ofile.flush();
            ofile.close();
        }
    }


    private:

    const std::string get_default_config_file()
    {
        // create a default config file
        /*
        std::string default_config_string; //("set fontsize 11\nset cursorblinkrate 500 # millisecond");
        for(ConfigIterator_t it{_config_.begin()}; it != _config_.end(); ++ it)
        {
            default_config_string += it->second.GetStringDefault() + std::string("\n");
        }
        default_config_string.pop_back(); // remove final new line
        return default_config_string;
        */

        // New code
        std::string default_config_string;

        for(ConfigIntOptionMapIterator_t it{_config_int_option_map_.begin()}; it != _config_int_option_map_.end(); ++ it)
        {
            default_config_string += it->second.GetStringDefault() + std::string("\n");
        }

        for(ConfigIntOptionMapIterator_t it{_config_int_option_map_.begin()}; it != _config_int_option_map_.end(); ++ it)
        {
            default_config_string += it->second.GetStringDefault() + std::string("\n");
        }

        default_config_string.pop_back();

        return default_config_string;

    }

    void set_defaults()
    {
        //_font_size_ = _DEFAULT_font_size_;
        //_cursor_blink_rate_ = 500; // millisecond

        /*
        for(ConfigIterator_t it{_config_.begin()}; it != _config_.end(); ++ it)
        {
            it->second.SetDefault();
        }
        */

        // New code
        for(ConfigIntOptionMapIterator_t it{_config_int_option_map_.begin()}; it != _config_int_option_map_.end(); ++ it)
        {
            it->second.SetDefault();
        }
        
        // New code
        for(ConfigIntOptionMapIterator_t it{_config_int_option_map_.begin()}; it != _config_int_option_map_.end(); ++ it)
        {
            it->second.SetDefault();
        }


    }

    //int _font_size_;
    //int _cursor_blink_rate_;

    const std::string _filename_{"config.rc"};

    // map of commands to config options
    std::map<const std::string, ConfigOption<int>> _config_;
    typedef std::map<const std::string, ConfigOption<int>>::iterator ConfigIterator_t;


    // New code, new "hack" to make multiple types work without hcontainer
    // map to map
    // maps string (name) to map containing the correct type
    // second map maps string (name) to actual variable
    // EDIT: changed to using void* because template types do not work!
    std::map<const std::string, void*> _config_name_to_map_pointer_; // TODO: not used!
    std::map<const std::string, ConfigOption<int>> _config_int_option_map_;
    std::map<const std::string, ConfigOption<double>> _config_float_option_map_; // double not float
    typedef std::map<const std::string, ConfigOption<int>>::iterator ConfigIntOptionMapIterator_t;
    typedef std::map<const std::string, ConfigOption<double>>::iterator ConfigFloatOptionMapIterator_t;

};

#endif // CONFIG_HPP
