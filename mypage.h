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
        .digital {
            display: inline-block;
            width: 25px;
            height: 25px;
            margin: 15px;
            border: 20px solid var(--secondary-color);
            vertical-align: bottom;
        }
        .on {
            background-color: var(--primary-color);
            box-shadow: 0px 0px 10px var(--primary-color);
        }
        .off {
            background-color: white;
        }
        .HIGH {
            margin-top: 0px;
            border-color: transparent transparent var(--secondary-color) transparent;
        }
        .LOW {
            margin-bottom: 0px;
            border-color: var(--secondary-color) transparent transparent transparent;
        }
            /* 
            
            border-color: var(--secondary-color) transparent transparent transparent;
            border-color: transparent var(--secondary-color) transparent transparent;
            border-color: transparent transparent var(--secondary-color) transparent;
            border-color: transparent transparent transparent var(--secondary-color);
            
            */
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
            <a id='digital_builtin' class='button' onclick='digitalHigh(this)'>Builtin LED HIGH</a>
            <a id='digital_builtin' class='button' onclick='digitalLow(this)'>Builtin LED LOW</a>
            <a id='digital_builtin' class='button' onclick='digitalToggle(this)'>Builtin LED Toggle</a>
            <div id='digital_builtin' class='digital LOW'></div>

            <br>
            <a id='digital_4' class='button' onclick='digitalToggle(this)'>Pin 4</a>
            <div id='digital_4' class='digital LOW'></div>
            <br>
            <a id='digital_5' class='button' onclick='digitalToggle(this)'>Pin 5</a>
            <div id='digital_5' class='digital LOW'></div>
)=====";

const char MAIN_test[] PROGMEM = R"=====(
            <h2>Test Arguments</h2>
            <a id='testing' class='button' onclick='handleTest()'>Test POST args</a>
            
            <div id='input_with_btn'>
                <input id='str_testStr' class='attachVariable'><a id='str_testStr' class='button' onclick=''>Submit</a>
            </div>

            <h3>Delete Local Storage</h3>
            <a id='deleteLocalStorage' class='button' onclick='deleteLocalStorage()'>Delete Storage</a>
)=====";

const char MAIN_footer[] PROGMEM = R"=====(
        </main>
    </div>

    <script>
        var digitalDefault = 'LOW'; // change this to HIGH if you would like all digital variables to be set to HIGH by default

        function deleteLocalStorage() {
            localStorage.clear();
        }

        function getVars() {
            var varArr = document.getElementsByClassName('digital');
            for(var i = 0; i < varArr.length; i++) {
                if(varArr[i].id.split('_')[0] == 'digital') {
                    if(!localStorage[varArr[i].id]) {
                        localStorage[varArr[i].id] = digitalDefault;
                    }
                    console.log(varArr[i].id + ' is a digital');
                    handleDigital(varArr[i].id);
                } else {
                    console.log(varArr[i].id + ' is not digital');
                }
            }
        }

        function digitalToggle(thisEle) {
            var digital = thisEle.id;
            if(localStorage[digital] == 'LOW') {
                localStorage[digital] = 'HIGH';
            } else {
                localStorage[digital] = 'LOW';
            }
            handleDigital(digital);
        }

        function digitalLow(thisEle) {
            var digital = thisEle.id;
            localStorage[digital] = 'LOW';
            handleDigital(digital);
        }

        function digitalHigh(thisEle) {
            var digital = thisEle.id;
            localStorage[digital] = 'HIGH';
            handleDigital(digital);
        }

        function handleDigital(digital) {
            var digitalIO = document.getElementsByClassName('digital');
            for(var i = 0; i < digitalIO.length; i++) {
                if(digitalIO[i].id == digital) {
                    if(localStorage[digital] == 'HIGH') {
                        digitalIO[i].className = 'digital HIGH';
                    } else {
                        digitalIO[i].className = 'digital LOW';
                    }
                }
            }

            var digitalPin = digital.split('_')[1];
            var digitalStatus = localStorage[digital];
            console.log('Post Data: ' + 'digitalStatus=' + digitalStatus + '&digitalPin=' + digitalPin)
            var xhttp = new XMLHttpRequest();
            xhttp.open('POST', '/post', true);
            xhttp.send('digitalStatus=' + digitalStatus + '&digitalPin=' + digitalPin);
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