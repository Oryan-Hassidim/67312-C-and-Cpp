git checkout submit
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git checkout master -- Matrix.h Matrix.cpp Activation.h Activation.cpp Dense.h Dense.cpp MlpNetwork.h MlpNetwork.cpp
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
wsl clang-format -i -style=GNU Matrix.h Matrix.cpp Activation.h Activation.cpp Dense.h Dense.cpp MlpNetwork.h MlpNetwork.cpp
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git add -u
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git commit -m "Submit ex3.b"
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git push
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE 
}
git checkout master