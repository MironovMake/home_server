const setFavicon = function (url) {
    // Find the current favicon element
    const favicon = document.querySelector('link[rel="icon"]');
    if (favicon) {
        // Update the new link
        favicon.href = url;
    } else {
        // Create new `link`
        const link = document.createElement('link');
        link.rel = 'icon';
        link.href = url;

        // Append to the `head` element
        document.head.appendChild(link);
    }
};



// Если вы хотите динамически обновить значок, например, значок пользователя на сайте:

setFavicon('/path/to/user/profile/icon.ico');
// Используйте эмодзи в качестве иконки
// Обратите внимание, что функция setFavicon() принимает URL - адрес значка сайта.Мы можем получить что - то интересное, передав пользовательский URL.
// 
// В приведенном ниже коде мы создаем canvas элемент, заполняем его определенным эмодзи и получаем пользовательский URL:

const emojiFavicon = function (emoji) {
    // Create a canvas element
    const canvas = document.createElement('canvas');
    canvas.height = 64;
    canvas.width = 64;

    // Get the canvas context
    const context = canvas.getContext('2d');
    context.font = '64px serif';
    context.fillText(emoji, 0, 64);

    // Get the custom URL
    const url = canvas.toDataURL();

    // Update the favicon
    setFavicon(url);
};

// Usage
emojiFavicon('🎉');