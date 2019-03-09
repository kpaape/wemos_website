const char MAIN_header[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8' />
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <title>Page Title</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
        * {
            font-family: sans-serif;
            text-decoration: none;
            box-sizing: border-box;
            /* border: 1px solid red; */
        }
        :root {
            --primary-color: rgb(20, 200, 260);
            --secondary-color: #00979d;
            --background-color: white;
        }
        body {
            background-color: var(--primary-color);
            margin: 0;
        }
        main, header {
            padding: 20px;
        }
        header {
            background-color: var(--secondary-color);
            color: white;
        }
        nav {
            background-color: var(--secondary-color);
            text-align: center;
        }
        nav ul {
            list-style: none;
            margin: 0;
            padding: 0;
        }
        nav ul li {
            display: inline-block;
            padding: 10px;
        }
        nav ul li a {
            color: white;
        }
        input {
            padding: 9px;
            width: 100%;
            border: 2px solid var(--secondary-color);
            vertical-align: bottom;
        }
        .button {
            display: inline-block;
            padding: 10px;
            margin: 10px 0;
            color: white;
            background-color: var(--secondary-color);
            text-align: center;
            cursor: pointer;
            vertical-align: bottom;
        }
        #container {
            width: 800px;
            max-width: 90vw;
            margin: 20px auto;
            background-color: white;
        }
        #input_with_btn input {
            width: 70%;
            height: 40px;
            margin: 0;
        }
        #input_with_btn .button {
            width: 30%;
            height: 40px;
            margin: 0;
        }
        .led {
            display: inline-block;
            width: 25px;
            height: 25px;
            margin: 15px;
            border: 3px solid var(--secondary-color);
            border-radius: 25px;
            vertical-align: bottom;
        }
        .on {
            background-color: var(--primary-color);
            box-shadow: 0px 0px 10px var(--primary-color);
        }
        .off {
            background-color: white;
        }
    </style>
</head>
<body>
    <div id='container'>
        <header>
            <h1>Wemos D1 R2</h1>
        </header>
        <nav>
            <ul>
                <li>
                    <a href='/'>Home</a>
                </li><!--
                --><li>
                    <a href='/test_arguments'>Test Page</a>
                </li>
            </ul>
        </nav>
        <main>

)=====";

const char MAIN_index[] PROGMEM = R"=====(
            <!-- INDEX -->
            <a id='led_builtin' class='button' onclick='toggleLed(this)'>Builtin LED Toggle</a>
            <a id='led_builtin' class='button' onclick='turnOnLed(this)'>Builtin LED On</a>
            <a id='led_builtin' class='button' onclick='turnOffLed(this)'>Builtin LED Off</a>
            <div id='led_builtin' class='led off'></div>

            <br>
            <a id='led_4' class='button' onclick='toggleLed(this)'>LED on pin 4</a>
            <div id='led_4' class='led off'></div>
            <br>
            <a id='led_5' class='button' onclick='toggleLed(this)'>LED on pin 5</a>
            <div id='led_5' class='led off'></div>
)=====";

const char MAIN_test[] PROGMEM = R"=====(

            <!-- TEST PAGE -->
            <h2>Test Arguments</h2>
            <a id='testing' class='button' onclick='handleTest()'>Test POST args</a>
            
            <div id='input_with_btn'>
                <input id='str_testStr' class='attachVariable'><a id='str_testStr' class='button' onclick=''>Submit</a>
            </div>
)=====";

const char MAIN_footer[] PROGMEM = R"=====(
        </main>
    </div>

    <script>
        function getVars() {
            var varArr = document.getElementsByClassName('led');
            for(var i = 0; i < varArr.length; i++) {
                if(varArr[i].id.split('_')[0] == 'led') {
                    if(!localStorage[varArr[i].id]) {
                        localStorage[varArr[i].id] = 'off';
                    }
                    console.log(varArr[i].id + ' is an led');
                    handleLed(varArr[i].id);
                } else {
                    console.log(varArr[i].id + ' is not an led');
                }
            }
        }

        function toggleLed(thisEle) {
            var led = thisEle.id;
            if(localStorage[led] == 'off') {
                localStorage[led] = 'on';
            } else {
                localStorage[led] = 'off';
            }
            handleLed(led);
        }

        function turnOffLed(thisEle) {
            var led = thisEle.id;
            localStorage[led] = 'off';
            handleLed(led);
        }

        function turnOnLed(thisEle) {
            var led = thisEle.id;
            localStorage[led] = 'on';
            handleLed(led);
        }

        function handleLed(led) {
            var leds = document.getElementsByClassName('led');
            for(var i = 0; i < leds.length; i++) {
                if(leds[i].id == led) {
                    if(localStorage[led] == 'on') {
                        leds[i].className = 'led on';
                    } else {
                        leds[i].className = 'led off';
                    }
                }
            }

            var ledPin = led.split('_')[1];
            var ledStatus = localStorage[led];
            console.log('Post Data: ' + 'ledStatus=' + ledStatus + '&ledPin=' + ledPin)
            var xhttp = new XMLHttpRequest();
            xhttp.open('POST', '/post', true);
            xhttp.send('ledStatus=' + ledStatus + '&ledPin=' + ledPin);
        }

        function handleTest() {
            var xhttp = new XMLHttpRequest();
            xhttp.open('POST', '/handle_test', true);
            xhttp.send('cheese=burger&ham=sandwich');
        }
        
        if(typeof(Storage) != 'undefined') {
            console.log('Saving compatible');

            getVars();

            console.log(localStorage);
        } else {
            console.log('Not compatible');
        }
    </script>
</body>
</html>
)=====";