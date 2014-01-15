var config_data = {
  'PANEL': {
    'API_URL': 'http://voip.symplicity.com/api-devel'
  }
}
var callstatsApp = angular.module('config',function(){
	angular.forEach(config_data,function(key,value) {
  		callstatsApp.constant(value,key);
	});
});
