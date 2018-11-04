# Multi Greeting Contract
This is just a small and simple implementation using eosio `multi_index`es in order to persist storage. It is meant to be a very simple example showing how this is done in eos.

To learn more about how `multi_index` works see the code comments in `multigreet.cpp`.

## Usage

### deployment
create an account for the contract:
```sh
cleos create account eosio multigreeting $EOS_PUBLIC_KEY
```

compile the contract using the account (in the root dir):
```sh
eosio-cpp -o multigreet.wasm multigreet.cpp --abigen
```

deploy the contract:
```sh
cleos set contract multigreet $EOS_CONTRACT_DIR/multigreet
```

### Get the Default Greeting
```sh
cleos push action multigreet greet '["bob"]' -p bob
```
you should get the following back:
```sh
>> Hello bob
```

### Set a Personalized Greeting
set a greeting for an account (ex. bob):
```sh
cleos push action multigreet cgreeting '["bob", "yo bob!"]' -p bob
```

get the greeting:
```sh
cleos push action multigreet greet '["bob"]' -p bob
```
you should get the following back:
```sh
>> yo bob!
```

### Update the Personalized Greeting
update the greeting:
```sh
cleos push action multigreet ugreeting '["bob", "hello dearest sir Bobbington the 2nd, I hope you are having a splendid day"]' -p bob
```
get the greeting:
```sh
cleos push action multigreet greet '["bob"]' -p bob
```
you should get the following back:
```sh
>> hello dearest sir Bobbington the 2nd, I hope you are having a splendid day
```

### Delete the Personalized Greeting
delete the greeting:
```sh
cleos push action multigreet dgreeting '["bob"]' -p bob
```
get default greeting after deletion:
```sh
cleos push action multigreet greet '["bob"]' -p bob
```
you should get the following back:
```sh
>> Hello bob
```


