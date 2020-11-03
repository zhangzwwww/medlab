# Frontend pc

This folder gives the front end implemention of the project.

## Notes

`./frontend_pc.pro` is the QT parent project file. In the QT creator will build both `./App/App.pro` and `./Test/Test.pro`. Thus, please remember not to change the relative path of these two files during development.

`./App/` is the source code of the project. For now the team has two ways of building the project. 

- If you are using `cmake` , construct your `CMakeLists.txt` in `./` or `./App/` . Remember set the building output path in a saparate folder, for example `./App/build/`. 
- If you are using QT creator and `qmake` to build projects, just remember to check the building output path. When building, remember to check the source code path in project files, such as `./App/App.pro`. Further, as different platforms may have different encoding for Chinese. Plz set the encoding mode of your editor as `GBK`, if there's something wrong when displaying Chinese.

Again, be careful when changing the path of files. Rember to ensure the project building works when doing so.