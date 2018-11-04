#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;
using std::string;

class multigreet : public contract {
  public:
    using contract::contract;

    // this gives us access to _self which is a name type
    multigreet(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

    // [[eosio::action]] is needed for all actions in order to ensure the abi generator works
    [[eosio::action]]
    void cgreeting(name account, string &greeting_text) {
      // ensure that the account calling this function is the same as the entry we are trying to change (remember greetings are personalized)
      require_auth(account);
      // setup the scope of the greeting_index
      greeting_index greetings(_self, _self.value);
      // search for entry by primary key (account.value)
      auto iterator = greetings.find(account.value);
      // we check here that we run through the entire iterator without finding an entry matching our primary key
      eosio_assert(iterator == greetings.end(), "Greeting has already been created for account. Use ugreeting instead.");
      // [&](auto &greeting) {...} is a lambda!
      // [&] indicates that all is being passed by reference
      // (auto &greeting) means we are passing in greeting from emplace()
      greetings.emplace(account, [&](auto &greeting) {
        // now that we are inside the lambda and have access to greeting, assign the data
        greeting.account = account;
        greeting.greeting_text = greeting_text;
      });
    }

    [[eosio::action]]
    void ugreeting(name account, string &greeting_text) {
      require_auth(account);
      greeting_index greetings(_self, _self.value);
      auto iterator = greetings.find(account.value);
      // here we do the opposite of cgreeting and ensure that a record DOES exist
      eosio_assert(iterator != greetings.end(), "No greeting found for account. Use cgreeting instead.");

      // we need to pass in iterator in order to modify... otherwise very much the same
      greetings.modify(iterator, account, [&](auto &greeting) {
        // assign the data we want to inside of the lambda
        greeting.greeting_text = greeting_text;
      });
    }

    [[eosio::action]]
    void dgreeting(name account) {
      require_auth(account);
      greeting_index greetings(_self, _self.value);
      auto iterator = greetings.find(account.value);
      eosio_assert(iterator != greetings.end(), "No greeting found for account.");
      // we only need the non end() iterator here in order to erase
      greetings.erase(iterator);
    }

    [[eosio::action]]
    void greet(name account) {
      require_auth(account);
      greeting_index greetings(_self, _self.value);
      auto iterator = greetings.find(account.value);

      if (iterator == greetings.end()) {
        // print a default greeting if no personalized greeting is found
        print("Hello ", name{account});
      } else {
        // if a personalized greeting does exist we retrieve it using get() with the primary key (account.value)
        greeting personalized_greeting = greetings.get(account.value);
        // use our personalized greeting entry by accessing greeting_text
        print(personalized_greeting.greeting_text);
      }
    }

  private:
    // the table struct used for multi_index can be thought of as some sort of a database schema... we are defining what we are going to save in the table here
    // we need to add [[eosio::table]] in order for the abi generator to pick it up
    struct [[eosio::table]] greeting
    {
      name account; // the account this greeting is for (this will be unique)
      string greeting_text; // the actual greeting text to print for an account when calling greet()
      uint64_t primary_key() const { return account.value; } // this is required for any multi_index, here we are setting account.value as our primary key
      EOSLIB_SERIALIZE(greeting, (account)(greeting_text)) // erm... to be honest I am a noob... not a clue what this macro does...
    };

    // define the type here rather than writing out this long complicated type each time we need to use it
    typedef multi_index<"greetings"_n, greeting> greeting_index;
};

// this macro applies actions to the address code
EOSIO_DISPATCH(
  multigreet, 
  (cgreeting)
  (ugreeting)
  (dgreeting)
  (greet)
)