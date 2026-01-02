#pragma once

namespace sandbox {

bool apply_default_policy();
void revoke_policy();
bool is_landlock_available();

} // namespace sandbox
