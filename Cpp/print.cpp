// DEBUG print
std::cout << "AN>> " << __FILE__ << ", L" <<  __LINE__ << ": " << __FUNCTION__ << ":\n"
          << "value = '" << value << "', "
          << "filename = '" << filename << "'"
          << '\n';

// NSXLOG
nsxlog(nsx::Level::Debug, "AN>> ", __FILE__, ":L", __LINE__, ":", __FUNCTION__, ": ", "fileBasename = '", _metadata.key<std::string>(nsx::at_filepath), "'");

// Variant DEBUG print
std::cout << "AN>> " << __FILE__ << ", L" <<  __LINE__ << ": " << __FUNCTION__ << ": cumulative metadata\n";
        for (const auto& [key, val] : metadata.map()) {
            std::cout << "key = '" << key << "', ";
            if (std::holds_alternative<int>(val)) {
                std::cout << "value<int> = " << std::get<int>(val) << "\n";
            }
            else if (std::holds_alternative<double>(val)) {
                std::cout << "value<double> = " << std::get<double>(val) << "\n";
            }
            else if (std::holds_alternative<std::string>(val)) {
                std::cout << "value<string> = " << std::get<std::string>(val) << "\n";
            } else {
                std::cout << "value<?> = ?\n";
            }
        }
        std::cout << std::endl;
