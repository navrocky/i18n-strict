# i18n::strict — a library for strict typed internationalization

A strictly typed, lightweight internationalization and localization system for multilingual programs. 

## Main features

- Dictionaries are written in code, and the presence and correctness of translations are checked by the compiler
- Built-in support for string formatting based on std::format
- Static verification of the required number of substitutions in the translated strings
- Clean code without singletons and other magic
- Support for dynamic translation switching. This can be useful, for example, in HTTP request handlers
- Support for lazy translation. This allows you to generate a translatable string once, and then, for example, 
  translate it into the system language in the error handler during logging, and return it in the request language 
  in the server response.
- A library with a single header makes it easy to add translations to your project

## Requirements and dependencies

- C++20
- No dependencies requires

## Examples of usage

```cpp
#include <lib/i18n_strict.h>

struct test_dict : public i18n_strict::dict {
    str<> app_started;
    str<int, std::string> transfer_money_to_account;
};

test_dict create_en_test_dict()
{
    return {
        .app_started { "Application started" },
        .transfer_money_to_account { "Transfer {} to account {}" },
    };
}

test_dict create_ru_test_dict()
{
    return {
        .app_started { "Приложение запущено" },
        .transfer_money_to_account { "Перевести {} на счет {}" },
    };
}

auto tr = i18n_strict::create_text_translator<test_dict>("ru",
    {
        { "en", create_en_test_dict() },
        { "ru", create_ru_test_dict() },
    });
    
TEST_CASE("Translation")
{
    auto tr = i18n_strict::create_text_translator<test_dict>("ru",
        {
            { "en", create_en_test_dict() },
            { "ru", create_ru_test_dict() },
        });

    SECTION("Translate simple text to english")
    {
        REQUIRE(tr->translate("en", &test_dict::app_started) == "Application started");
    }
    SECTION("Translate simple text to default language")
    {
        REQUIRE(tr->translate(&test_dict::app_started) == "Приложение запущено");
    }
    SECTION("Translate formatted message to english")
    {
        REQUIRE(tr->translate("en", &test_dict::transfer_money_to_account, 10, std::string("John"))
            == "Transfer 10 to account John");
    }
    SECTION("Use translatable string to translate for specific language later")
    {
        i18n_strict::tr_str str(tr, &test_dict::transfer_money_to_account, 10, std::string("John"));
        REQUIRE(str.translate("en") == "");
        REQUIRE(str.translate() == "");
    }
}
```

## Installation

Just copy and include `lib/i18n_strict.h` in your project.

## Backlog

- [ ] Describe installation with [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html)
- [ ] Add package to [vcpkg](https://vcpkg.io)
- [ ] Add package to [Conan Center](https://conan.io/center)
