# PhotoPainter (B)

## Custom firmware

While the original firmware is adequate for some photos, I think we can do a bit better.

So this firmware has the following changes:
- Added mode 3 that will randomly select an image. It supports as many as you can fit on your SD card (FAT32). Tested with 2000+ images without issue.
- Added config.txt. Including this on the root of your SD card with the following `3,1800,3600` will configure Mode 3 with a minimum refresh rate of 1800 seconds (half an hour) and a maximum of 3600 seconds (an hour). If you don't want variable refresh rates, use the same value for min/max
- Because of this, only one firmware file should be sufficient for all use cases. Check the releases page

### Using this
1. Add config.txt with `3,1800,3600` to root of SD card (or whatever seconds you'd like, should be pretty flexible). Then add all your pictures to `pic` folder. 
2. Press boot and run button (small buttons) together and release, this should expose the picture frame as a flash drive. Drag the uf2 file into the folder.
3. Press run button after flash drive disappears. If it doesn't automatically start displaying an image, push the next button. Done!

## Bonus
### Picture preparation

While the instructions work, I've found that the contrast and saturation is lacking. For best results, boost the saturation, contrast and brightness. Or using something like PIL for Python: 

```
  enhanced_image = ImageEnhance.Contrast(processed_image).enhance(1.2)
  enhanced_image = ImageEnhance.Brightness(enhanced_image).enhance(1.1)
  enhanced_image = ImageEnhance.Color(enhanced_image).enhance(1.5)
```

I've included a python script for convenience. Thanks @matthewbadeau for the original script!

### Obligatory excuses

The original code has some unreadable/messy parts, therefore I took the liberty of some shortcuts to not have to deal with those. Shouldn't affect the functionality, only the readability of the code.

## Waveshare Electronics
waveshare electronics</br>
![waveshare_logo.png](waveshare_logo.png)

---

## 中文说明

本例程为 PhotoPainter (B) 的示例代码，友情开源，有bug欢迎反馈。

相关资料地址：  
[PhotoPainter - Waveshare Wiki](https://www.waveshare.net/wiki/PhotoPainter)  

## 图片制作

一、使用 Photoshop 制作散点图，然后导入到 SD 卡。参考教程：  
[E-Paper-Floyd-Steinberg - Waveshare Wiki](https://www.waveshare.net/wiki/E-Paper-Floyd-Steinberg)  

三、使用我们提供的六色抖动图片转换工具。下载链接：  
[PhotoPainter_B-Floyd.zip](https://www.waveshare.net/w/upload/5/5b/ConverTo6c_bmp-7.3.zip)  

### 图片转换工具使用

#### Windows 系统

- **单张图片转换**  
  将`图片`拖到 `convert.exe` 上即可完成转换。

- **批量转换图片**  
  将`图片`、`convert.exe` 和 `converterTo6color_all.cmd` 放在同一个文件夹中，然后双击 `converterTo6color_all.cmd` 文件即可进行批量转换。

#### MAC 系统

将`图片`、`convert`、`converterTo6color_all` 放在一个文件夹

打开终端，进入对应的文件夹
> cd xxxx/xxx/xxx/

- **单张图片转换**  
  输入命令，给予 `convert` 可执行权限
  >sudo chmod +x convert
  
  转换图片
  >./convert 图片名称

- **批量转换图片**  
  要先对 `convert` 文件进行权限操作给予后才行
  
  输入命令，给予 `converterTo6color_all.sh` 可执行权限
  >sudo chmod +x converterTo6color_all.sh

  转换图片
  >./converterTo6color_all.sh

---

## English

This example code is for PhotoPainter (B), which is open-sourced in a friendly manner. If you find any bugs, please feel free to report them.

Related documentation can be found at:  
[PhotoPainter - Waveshare Wiki](https://www.waveshare.com/wiki/PhotoPainter)  

## Image Preparation

1.Using Photoshop to create a dithered image and then importing it to an SD card. For a tutorial, please refer to:  
[E-Paper-Floyd-Steinberg - Waveshare Wiki](https://www.waveshare.com/wiki/E-Paper_Floyd-Steinberg)  

3.Using our provided 6-color dithering image conversion tool. Download link:  
[PhotoPainter_B-Floyd.zip](https://files.waveshare.com/wiki/common/ConverTo6c_bmp-7.3.zip) 

### How to Use the Image Conversion Tool

#### For Windows

- **Single Image Conversion**  
  Drag the image onto `convert.exe` to complete the conversion.

- **Batch Image Conversion**  
  Place the `images`, `convert.exe`, and `converterTo6color_all.cmd` in the same folder. Double-click the `converterTo6color_all.cmd` file to perform batch conversion.

#### For MAC

Place the `images`, `convert` and `converterTo6color_all` in the same folder.

Open the terminal and navigate to the corresponding folder:
> cd xxxx/xxx/xxx/

- **Single Image Conversion**  
  Grant execution permission to `convert`:
  >sudo chmod +x convert
  
  Convert the image:
  >./convert image_name

- **Batch Image Conversion**  
  You need to first grant execution permissions to the `convert` file.

  Grant execution permission to `converterTo6color_all.sh`:
  >sudo chmod +x converterTo6color_all.sh

  Convert the images:
  >./converterTo6color_all.sh

---
