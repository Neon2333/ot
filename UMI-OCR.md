### 1. 在Qt中调用UMI-OCR截图功能并获取识别文本

#### 实现步骤：

##### ① 监听全局快捷键

使用第三方库如`QHotkey`监听快捷键：

cpp

复制

```
#include <QHotkey>

QHotkey *hotkey = new QHotkey(QKeySequence("Ctrl+Shift+A"), true, this);
connect(hotkey, &QHotkey::activated, [this](){
    startScreenshotOCR();
});
```

##### ② 实现屏幕截图

使用Qt原生API实现区域截图：

cpp

复制

```
void MainWindow::startScreenshotOCR() {
    // 隐藏窗口避免干扰
    this->hide();
    QThread::msleep(300); // 等待窗口隐藏

    // 获取屏幕尺寸
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap fullPixmap = screen->grabWindow(0);
    
    // 显示截图区域选择窗口（需自定义）
    ScreenshotAreaSelector selector(fullPixmap);
    if (selector.exec() == QDialog::Accepted) {
        QRect selectedRect = selector.getSelectedArea();
        QPixmap screenshot = fullPixmap.copy(selectedRect);
        QString tempPath = QDir::tempPath() + "/umiocr_temp.png";
        screenshot.save(tempPath);
        runUmiOcr(tempPath);
    }
    
    this->show();
}
```

##### ③ 调用UMI-OCR命令行

使用QProcess执行命令行：

cpp

复制

```
void MainWindow::runUmiOcr(const QString &imagePath) {
    QProcess *process = new QProcess(this);
    QString exePath = "path/to/umiocr_cli.exe";
    
    // 构建命令参数
    QStringList args;
    args << "--image" << imagePath
         << "--output" << "txt"
         << "--quiet";  // 抑制非必要输出

    connect(process, &QProcess::readyReadStandardOutput, [process, this](){
        QByteArray output = process->readAllStandardOutput();
        QString result = QString::fromLocal8Bit(output);
        processOCRResult(result.trimmed());
    });

    connect(process, QOverload<int>::of(&QProcess::finished), [=](int code){
        if (code != 0) {
            QByteArray error = process->readAllStandardError();
            qDebug() << "OCR Error:" << error;
        }
        process->deleteLater();
    });

    process->start(exePath, args);
}
```

##### ④ 处理识别结果

cpp

复制

```
void MainWindow::processOCRResult(const QString &text) {
    if (!text.isEmpty()) {
        // 后续处理：显示结果或调用翻译API
        ui->textEdit->setPlainText(text);
        // 调用百度翻译接口...
    } else {
        QMessageBox::warning(this, "Error", "OCR识别失败");
    }
}
```

#### 关键配置说明：

1. 路径处理：

   - 建议将`umiocr_cli.exe`打包到应用目录的`/vendor`子目录
   - 使用`QCoreApplication::applicationDirPath()`获取绝对路径

2. 参数优化：

   cpp

   复制

   ```
   args << "--lang" << "eng+chs"  // 多语言识别
        << "--psm" << "6";       // 页面分割模式
   ```

3. 错误处理：

   cpp

   复制

   ```
   connect(process, &QProcess::errorOccurred, [](QProcess::ProcessError error){
       qDebug() << "Process Error:" << error;
   });
   ```

------

### 2. Qt调用UMI-OCR命令行详解

#### 核心方法：使用QProcess类

cpp

复制

```
QProcess process;
process.setProgram("umiocr_cli.exe");
process.setArguments({"--screenshot", "--clipboard"});

// 同步执行（阻塞式）
process.start();
process.waitForFinished(30000); // 30秒超时

// 异步执行（推荐）
process.start();
```

#### 典型使用场景：

##### ① 截图到剪贴板

cpp

复制

```
QStringList args;
args << "--screenshot" 
     << "--clipboard"
     << "--lang" << "ja";  // 识别日语

process.start("umiocr_cli.exe", args);
```

##### ② 识别现有图片文件

cpp

复制

```
QString imagePath = "C:/Users/Test/1.png";
QString outputPath = "C:/Users/Test/result.txt";

QStringList args;
args << "--image" << imagePath
     << "--output" << outputPath
     << "--format" << "json";

process.start("umiocr_cli.exe", args);
```

##### ③ 获取实时输出

cpp

复制

```
connect(&process, &QProcess::readyReadStandardOutput, [&](){
    QTextStream stream(process.readAllStandardOutput());
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        qDebug() << "OCR Output:" << line;
    }
});
```

#### 注意事项：

1. 路径空格处理：

   cpp

   复制

   ```
   QString path = "C:/Program Files/UMI-OCR/cli.exe";
   process.setNativeArguments("\"" + path + "\""); // Windows需要引号包裹
   ```

2. 工作目录设置：

   cpp

   复制

   ```
   process.setWorkingDirectory("C:/UMI-OCR"); // 设置CLI需要的上下文路径
   ```

3. 环境变量继承：

   cpp

   复制

   ```
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   env.insert("TESSDATA_PREFIX", "C:/UMI-OCR/tessdata");
   process.setProcessEnvironment(env);
   ```

4. 超时控制：

   cpp

   复制

   ```
   process.waitForFinished(30000); // 毫秒单位
   if (process.state() == QProcess::Running) {
       process.kill();
   }
   ```

#### 调试技巧：

- 使用`process.errorString()`获取错误描述

- 捕获标准错误输出：

  cpp

  复制

  ```
  connect(&process, &QProcess::readyReadStandardError, [&](){
      qDebug() << "Error:" << process.readAllStandardError();
  });
  ```

- 记录完整命令：

  cpp

  复制

  ```
  qDebug() << "Running:" << process.program() << process.arguments().join(" ");
  ```

建议结合UMI-OCR的[官方CLI文档](https://github.com/hiroi-sora/UMI-OCR/blob/main/docs/README_CLI.md)调整具体参数，不同版本可能有参数差异。对于大量图片处理，建议采用批处理模式并设置合理的线程控制。