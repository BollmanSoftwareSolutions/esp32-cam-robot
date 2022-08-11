document.addEventListener('DOMContentLoaded', function () {
  send('direction', 'stop');
  send('pwm', 192);
  send('quality', 'VGA');

  document.querySelectorAll('#nav-buttons > button').forEach((button) => {
    ['mousedown', 'touchstart'].forEach((eventName) => {
      button.addEventListener(eventName, function () {
        send(button.dataset['action'], button.dataset['value']);
      });
    });

    ['mouseup', 'touchend'].forEach((eventName) => {
      button.addEventListener(eventName, function () {
        send('direction', 'stop');
      });
    });

    button.addEventListener('contextmenu', (e) => e.preventDefault());
  });
});

function send(action, value) {
  var url = `${document.location.origin}/control?action=${action}${value ? '?value=' + value : ''}`;
  console.log('action url', url);
  fetch(url);
}