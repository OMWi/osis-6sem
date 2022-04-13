#!/usr/bin/awk -f

BEGIN {
   FS=", "
}
{
   name=$1
   amount=$2
   price=$3

   if (products_amount[name] == 0) {
      products_name[name] = name
   }
   products_totalprice[name] += amount*price
   products_amount[name] += amount
}
END {
   for (name in products_name) {
      amount=products_amount[name]
      total_price=products_totalprice[name]
      avg_price=total_price/amount
      printf "name=%s, amount=%s, avg_price=%s, total_price=%s\n", name, amount, avg_price, total_price
   }
}