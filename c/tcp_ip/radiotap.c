struct ieee80211_radiotap_header{
	u_int8_t it_version;
	u_int8_t it_pad;
	u_int16_t it_len;
	u_int32_t it_present;
}__attrubute__((__packed_));
