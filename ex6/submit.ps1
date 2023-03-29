$files = "HashMap.hpp", "Dictionary.hpp"

git checkout submit
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git checkout master -- $files
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
wsl clang-format -i -style=GNU $files
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git add -u
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git commit -m "Submit ex6"
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git push
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git checkout master