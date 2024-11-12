new Vue({
  el: '#vu-meter',
  data: {
    vu: 0
  },
  created() {
    setInterval(() => {
      fetch('/vu')
        .then(response => response.json())
        .then(data => {
          this.vu = data.vu;
        });
    }, 1000);
  },
  template: '<div>VU Meter: {{ vu }}</div>'
});