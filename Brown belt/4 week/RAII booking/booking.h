//
// Created by ar_kud on 29.08.2020.
//

#include <utility>

namespace RAII {
    template<typename Provider>
    class Booking {
        Provider *provider;
        int &counter;
    public:
        Booking(Booking &&other) : counter(other.counter){
            provider = std::move(other.provider);
            other.provider = nullptr;
        }

        void operator=(Booking &&other) {
            provider = std::move(other.provider);
            other.provider = nullptr;
            counter = std::move(other.counter);
        }

        Booking(Provider *provider, int &counter) : provider(provider), counter(counter) {

        }

        ~Booking() {
            if (provider)
                provider->CancelOrComplete(*this);
        }
    };
}
