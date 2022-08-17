var origin = document.location.origin;

document.addEventListener('DOMContentLoaded', function () {
  send('direction', 'stop');
  send('pwm', 192);
  send('quality', 'VGA');

  document.querySelectorAll('#nav-buttons > button').forEach((button) => {
    ['mousedown', 'touchstart'].forEach((eventName) => {
      button.addEventListener(eventName, function (event) {
        event.preventDefault();
        send(button.dataset['action'], button.dataset['value']);
      });
    });

    ['mouseup', 'touchend'].forEach((eventName) => {
      button.addEventListener(eventName, function (event) {
        event.preventDefault();
        send('direction', 'stop');
      });
    });

    button.addEventListener('contextmenu', (e) => e.preventDefault());
  });

  document.getElementById('stream').src = `${origin}:81/stream`;

  fetch(`${origin}/info`)
    .then((response) => {
      document.getElementById('header').innerHTML = `${response}`;
    })
    .catch((e) => console.log('error getting header info', e));
});

function send(action, value) {
  var url = `${origin}/${action}${value ? '?value=' + value : ''}`;
  console.log('action url', url);
  fetch(url);
}
