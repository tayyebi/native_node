#include <iostream>
#include "sandbox/ruleset.h"
#include "sandbox/sandbox.h"

int main() {
    std::cout << "landlock_test: checking Landlock availability...\n";
    if (!sandbox::is_landlock_available()) {
        std::cout << "SKIP: Landlock not available on this kernel (kernel >= 5.13 required).\n";
        return 0; // skip
    }

    sandbox::RulesetBuilder b;
    b.add_path("./");
    bool ok = b.create_ruleset();
    if (!ok) {
        std::cerr << "FAILED: create_ruleset returned false\n";
        return 2;
    }
    ok = b.apply();
    if (!ok) {
        std::cerr << "FAILED: apply returned false\n";
        return 3;
    }
    std::cout << "PASS: Landlock ruleset created and applied (skeleton)\n";
    return 0;
}
