#pragma once
class GaiaInput
{
private:
    GaiaInput(); // Disallow instantiation outside of the class.
public:
    GaiaInput(const GaiaInput&) = delete;
    GaiaInput& operator=(const GaiaInput&) = delete;
    GaiaInput(GaiaInput&&) = delete;
    GaiaInput& operator=(GaiaInput&&) = delete;

    static auto& instance() {
        static GaiaInput gaiaInput;
        return gaiaInput;
    }

    void init();
    void close();


};

