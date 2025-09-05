// clazy:excludeall=non-pod-global-static
#include <catch2/catch_all.hpp>

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
    SECTION("Use translatable string to translate later")
    {
        i18n_strict::tr_str str(tr, &test_dict::transfer_money_to_account, 10, "John");
        REQUIRE(str.translate("en") == "Transfer 10 to account John");
        REQUIRE(str.translate() == "Перевести 10 на счет John");
    }
}
