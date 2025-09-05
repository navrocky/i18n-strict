#pragma once

#include <format>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace i18n_strict {

/**
 * @brief Dictionary string
 */
template <typename... Args>
class dict_str : public std::format_string<Args...> {
public:
    inline dict_str(std::format_string<Args...> text)
        : std::format_string<Args...>(text)
    {
    }
};

/**
 * @brief Base dictionary class
 *
 * This is a helper class
 */
struct dict {
    /**
     * String helper alias
     */
    template <typename... Args>
    using str = dict_str<Args...>;
};

/**
 * @brief Text translator
 */
template <typename Dict>
class text_translator {
public:
    using dict_type = Dict;
    using dicts_type = std::map<std::string, Dict>;
    template <typename... Args>
    using dict_field_ref_type = dict_str<Args...> Dict::*;

    /**
     * @brief text_translator
     * @param default_lang_code
     * @param dicts
     */
    text_translator(const std::string& default_lang_code, dicts_type&& dicts)
        : default_lang_code(default_lang_code)
        , dicts(std::move(dicts))
    {
    }

    /**
     * @brief translate
     * @param lang_code
     * @param dict_field_ref
     * @param args
     * @return
     */
    template <typename... Args>
    [[nodiscard]] std::string translate(const std::string& lang_code, dict_field_ref_type<Args...> dict_field_ref,
        std::convertible_to<Args> auto&&... args) const
    {
        std::string res;
        if (!lang_code.empty()) {
            auto it = dicts.find(lang_code);
            if (it != dicts.end()) {
                res = std::format(it->second.*dict_field_ref, std::forward<Args>(args)...);
            }
        }
        if (res.empty()) {
            auto it = dicts.find(default_lang_code);
            if (it != dicts.end()) {
                res = std::format(it->second.*dict_field_ref, std::forward<Args>(args)...);
            }
        }
        if (res.empty() && !dicts.empty()) {
            res = std::format(dicts.begin()->second.*dict_field_ref, std::forward<Args>(args)...);
        }
        return res;
    }

    /**
     * @brief translate
     * @param dict_field_ref
     * @param args
     * @return
     */
    template <typename... Args>
    [[nodiscard]] std::string translate(dict_field_ref_type<Args...> dict_field_ref, Args&&... args) const
    {
        return translate(std::string(), dict_field_ref, std::forward<Args...>(args)...);
    }

private:
    std::string default_lang_code;
    dicts_type dicts;
};

template <typename Dict>
using text_translator_ptr = std::shared_ptr<text_translator<Dict>>;

template <typename Dict>
[[nodiscard]] text_translator_ptr<Dict> create_text_translator(
    const std::string& default_lang_code, typename text_translator<Dict>::dicts_type&& dicts)
{
    return std::make_shared<text_translator<Dict>>(
        default_lang_code, std::forward<typename text_translator<Dict>::dicts_type>(dicts));
}

/**
 * @brief Translatable string
 */
class tr_str {
public:
    template <typename Dict, typename... Args>
    tr_str(const text_translator_ptr<Dict>& translator, dict_str<Args...> Dict::* dict_field_ref,
        std::convertible_to<Args> auto&&... args)
    {
        tr = [translator, dict_field_ref, ... args = std::move(args)](const std::string& lang_code) {
            return translator->translate(lang_code, dict_field_ref, (Args)args...);
        };
    }

    std::string translate(const std::string& lang_code = std::string()) const { return tr(lang_code); }

private:
    std::function<std::string(const std::string&)> tr;
};

}
